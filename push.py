#!/usr/bin/env python3
"""
Auto-commit + push with an AI-generated commit message.

Edit PROJECT_DIR below (leave empty to use the current directory).

Env vars (required for AI):
  AI_BASE_URL   OpenAI-compatible base URL, e.g. https://api.openai.com/v1
  AI_API_KEY    API key
  AI_MODEL      model name, e.g. gpt-4o-mini

Env vars (optional):
  GIT_REMOTE    default: origin
  GIT_BRANCH    default: current branch
  AI_REVIEW     set to "1" to confirm before push
"""
import os
import subprocess
import sys
from pathlib import Path

import requests

# ------------------------------------------------------------------
# 👇 EDIT THIS — path to the project you want to push
#    Leave empty ("") to use the current working directory.
# ------------------------------------------------------------------
PROJECT_DIR = "/storage/emulated/0/AideProjects/bs-mod"
# ------------------------------------------------------------------

MAX_DIFF_CHARS = 12000


def git(repo, *args, check=True):
    return subprocess.run(
        ["git", *args], cwd=repo, text=True, capture_output=True, check=check
    )


def resolve_repo(project_dir: str) -> Path:
    raw = project_dir.strip() or os.getcwd()
    p = Path(raw).expanduser().resolve()
    if not p.is_dir():
        print(f"Not a directory: {p}")
        sys.exit(1)
    try:
        root = git(p, "rev-parse", "--show-toplevel").stdout.strip()
    except subprocess.CalledProcessError:
        print(f"Not a git repository: {p}")
        sys.exit(1)
    return Path(root)


def get_ai_config():
    base_url = os.environ.get("AI_BASE_URL", "").rstrip("/")
    api_key = os.environ.get("AI_API_KEY", "")
    model = os.environ.get("AI_MODEL", "")
    missing = [k for k, v in
               [("AI_BASE_URL", base_url), ("AI_API_KEY", api_key), ("AI_MODEL", model)]
               if not v]
    if missing:
        print("Missing env vars: " + ", ".join(missing))
        sys.exit(1)
    return base_url, api_key, model


def generate_message(diff, stat, base_url, api_key, model):
    if len(diff) > MAX_DIFF_CHARS:
        diff = diff[:MAX_DIFF_CHARS] + "\n\n[... diff truncated ...]"

    prompt = (
        "You are a Git commit message generator. Based on the changes below, "
        "write a concise, informative commit message.\n"
        "Rules:\n"
        "- Use imperative mood (Add, Fix, Update, Remove, Refactor...)\n"
        "- First line max 72 chars\n"
        "- If multiple logical changes, add a blank line then 2-5 bullet points\n"
        "- Output ONLY the commit message, no quotes, no prefixes\n\n"
        f"Files changed:\n{stat}\n\n"
        f"Diff:\n{diff}"
    )

    resp = requests.post(
        f"{base_url}/chat/completions",
        headers={
            "Authorization": f"Bearer {api_key}",
            "Content-Type": "application/json",
        },
        json={
            "model": model,
            "messages": [{"role": "user", "content": prompt}],
            "temperature": 0.3,
        },
        timeout=90,
    )
    resp.raise_for_status()
    text = resp.json()["choices"][0]["message"]["content"].strip()
    return text.strip('"').strip("`").strip()


def main():
    repo = resolve_repo(PROJECT_DIR)
    print(f"Repository: {repo}")

    base_url, api_key, model = get_ai_config()

    branch = os.environ.get("GIT_BRANCH") or git(repo, "branch", "--show-current").stdout.strip()
    if not branch:
        print("Could not determine current branch (detached HEAD?).")
        sys.exit(1)
    remote = os.environ.get("GIT_REMOTE", "origin")

    if not git(repo, "status", "--porcelain").stdout.strip():
        print("Working tree clean. Nothing to commit.")
        return

    print("Staging changes...")
    git(repo, "add", "-A")

    diff = git(repo, "diff", "--cached").stdout
    stat = git(repo, "diff", "--cached", "--stat").stdout
    if not diff.strip():
        print("No staged diff. Aborting.")
        return

    print(f"Generating commit message with {model}...")
    try:
        message = generate_message(diff, stat, base_url, api_key, model)
    except requests.HTTPError as e:
        print(f"API error: {e}")
        if e.response is not None:
            print(e.response.text)
        sys.exit(1)
    except Exception as e:
        print(f"Failed to generate message: {e}")
        sys.exit(1)

    if not message:
        print("AI returned an empty message. Aborting.")
        git(repo, "reset", check=False)
        return

    print("\n--- Commit message ---")
    print(message)
    print("----------------------\n")

    if os.environ.get("AI_REVIEW") == "1":
        ans = input("Commit and push? [Y/n] ").strip().lower()
        if ans and ans != "y":
            print("Aborted by user.")
            git(repo, "reset", check=False)
            return

    print("Committing...")
    try:
        git(repo, "commit", "-m", message)
    except subprocess.CalledProcessError as e:
        print("Commit failed:")
        print(e.stderr or e.stdout)
        sys.exit(1)

    print(f"Pushing to {remote}/{branch}...")
    try:
        git(repo, "push", remote, branch)
    except subprocess.CalledProcessError as e:
        print("Push failed:")
        print(e.stderr or e.stdout)
        print(f"\nIf remote is ahead, run:")
        print(f"  git -C {repo} pull --rebase {remote} {branch}")
        sys.exit(1)

    print("✅ Done — committed and pushed.")


if __name__ == "__main__":
    main()