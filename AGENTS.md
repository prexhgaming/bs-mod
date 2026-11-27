# Project notes

## Build
- `jni/` is the whole project (lib-only, no Gradle). Build with `ndk-build` from `jni/`.
- CI: `.github/workflows/build.yml` — `nttld/setup-ndk@v1` (local-cache), zips `libzyn-bs.so`, posts it to Telegram.

## Settings plumbing (easy to break silently)
- The injected script (`jni/include/cpp/srcpy.cpp`, the `srcpy` raw string) reads **only** `data.json`. There is no UDP settings channel — do not add one back.
- C++ writes it from `jni/include/sync_with_py.h` into `GetConfigDir()` = `ANativeActivity::internalDataPath + "/Nexora/"`. Internal dir only, **no** `/storage/emulated/0` fallback: a fallback makes C++ and the script resolve different paths, so `sync_attrs` stays empty and every feature whose Python default is `False` (`bAIM`, `bESP_Skeleton`) looks broken while `bESP` (default `True`) still draws.
- `data.json` is unlinked at activity creation (`sync_with_py_start_cleanup`) and rewritten on the first frame after `LoadConfig()`, so stale values never survive a launch.
