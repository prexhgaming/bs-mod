# bs-mod — lib-only NDK builder (no VirtualApp, no Gradle)

This repo used to be an Android Studio Gradle project with a broken VirtualApp loader. It is now a standalone native‑code build that produces `libzyn-bs.so` via `ndk-build`.

## What changed
- **Dropped** the `app/` and `VirtualApp/` Gradle modules, all Java/Kotlin source, the Android manifest, and the `proguard-rules.pro`.
- **Moved** every native source file from `app/src/main/cpp/` into a top‑level `jni/` folder.
- **Kept** the original `Android.mk` and `Application.mk` unchanged (identical to the repo's original `HEAD`).
- **Removed** `injector.cpp` and its module (it was a separate ptrace injector, not needed here).
- **Output** is a single `.so` per ABI (currently only `armeabi-v7a`).

## Build output
After `ndk-build` runs, the shared library is at:
```
libs/armeabi-v7a/libzyn-bs.so
```
The Dobby prebuilt static library (`libdobby.a`) is also linked in.

## GitHub Actions build + Telegram delivery

A workflow at `.github/workflows/build.yml` is provided. On every push to `main` (or manual `workflow_dispatch`), it:

1. Checks out the repo.
2. Installs NDK r29 (via the pre‑installed NDK on the GitHub‑hosted runner).
3. Runs `ndk-build` in the repo root.
4. Zips `libs/armeabi-v7a/libzyn-bs.so`.
5. Posts the zip as a document to a Telegram chat via the Bot API.
6. Fails clearly if the required secrets are missing.

### Required GitHub repository secrets
| Name | Description |
|------|-------------|
| `TELEGRAM_BOT_TOKEN` | Your Telegram bot token (from `@BotFather`). |
| `TELEGRAM_CHAT_ID` | The numeric chat ID (or `@username`) where the zip will be sent. |

These **must not** be committed to the repo — keep them as secret variables in the repo settings.

## Local build (optional)
If you have the Android NDK installed locally:

```bash
export ANDROID_NDK_ROOT=/path/to/ndk
cd /workspace
ndk-build
# libs/armeabi-v7a/libzyn-bs.so will appear
```

## License
See `LICENSE` for details.