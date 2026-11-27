# Project notes

## Build
- `jni/` is the whole project (lib-only, no Gradle). Build with `ndk-build` from `jni/`.
- CI: `.github/workflows/build.yml` — downloads the official NDK r27d zip and unpacks it with `unzip`, then runs `ndk-build` with output teed to `build.log`. On success it uploads/sends `libzyn-bs.zip`; on failure it sends `build.log` instead (never both). `pipefail` is what makes a failed build fail the step — without it `tee` masks the status.
- Do **not** switch CI back to an NDK setup action (e.g. `nttld/setup-ndk`): those unpack the archive with a JS extractor that drops symlinks, so `clang++` (a symlink to `clang`) is dangling and every compile fails with `clang++: No such file or directory`. `unzip` preserves the symlinks.
- CI: `.github/workflows/cleanup.yml` — weekly cron (Sun 03:00 UTC) that deletes artifacts older than 7 days, caches unused for 7 days, and all but the newest 5 runs per workflow. Manual run takes `keep_runs`, `artifact_days`, `cache_days`, `keep_ndk_cache`, `dry_run`. Only `completed` runs are deleted, so in-progress ones are never touched, and the `ndk-*` cache is protected by default (it saves a ~660 MB download).

## Settings plumbing (easy to break silently)
- The injected script (`jni/include/cpp/srcpy.cpp`, the `srcpy` raw string) reads **only** `data.json`. There is no UDP settings channel — do not add one back.
- C++ writes it from `jni/include/sync_with_py.h` into `GetConfigDir()` = `ANativeActivity::internalDataPath + "/Nexora/"`. Internal dir only, **no** `/storage/emulated/0` fallback: a fallback makes C++ and the script resolve different paths, so `sync_attrs` stays empty and every feature whose Python default is `False` (`bAIM`, `bESP_Skeleton`) looks broken while `bESP` (default `True`) still draws.
- `data.json` is unlinked at activity creation (`sync_with_py_start_cleanup`) and rewritten on the first frame after `LoadConfig()`, so stale values never survive a launch.
