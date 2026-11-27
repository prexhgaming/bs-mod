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
- Config key names must match **exactly** on both sides. C++ writes `fESP_MaxDistance` (not `fESP_MaxDist`); a typo on the Python side silently falls back to the Python default.

## Debugging the injected script
- `@TRY` and `@HOOK` log rate-limited tracebacks via `_log_exc()` instead of `except: pass` — check logcat for `[NEXORA][ERR]` when a feature does nothing.
- `_esp_diag()` / `_aim_diag()` print `[NEXORA][ESP]` / `[NEXORA][AIM]` lines every 2 s: config key count, `bESP_Skeleton`, entity counts, bones collected, aim gate state, and the final `Rotate()` values. These say which stage stops. Aim also logs `stage bone=<n> proj=<n> fov_out=<n> ok=<n>`: `ok=0` with `bone>0` means bone lookups are failing, `proj>0` means the projection is.

## How ESP and aim are driven (neither is a direct engine hook)
- `AIMUpdate` is **not** decorated with `@HOOK`. The only driver is `on_space_tick`, registered through `Space._instance.AddSpacePostTick(...)` inside the `@HOOK(PlayerCombatAvatar)` `on_enter_space` wrapper. `on_space_tick` calls `EspUpdate()` (gated by `bESP`, default True) and `AIMUpdate()` (gated by `bAIM`, default False) — same callback, so if boxes draw, the chain is alive and the aim fault is downstream, not in the hook.
- Register through `_register_tick()`, never directly: it registers once per Space instance. Calling `AddSpacePostTick` on every `_on_enter_space` adds a duplicate callback each match and aim then applies its yaw/pitch delta once per copy per frame (overshoots, worse every match). Unit-tested in `uploads/test_register_tick.py`.
- `FpsPlacerInstance` comes from `@HOOK(FpsPlacer) OnStaticTick`; if that hook never fired the script falls back to `camera.fps_placer`, then to writing `camera.euler_angles` (`_USE_EULER`). `[NEXORA][AIM] run ... fpp=... euler=...` shows which path is live.

## Bones: ESP and aim are separate pipelines
- ESP skeleton collects bones only when `bESP_Skeleton` is on and the head is on screen; aim never uses that dict and does not need the toggle. Both depend on the engine's `entity.model.GetBoneWorldPosition(name)`.
- Never write `GetBoneWorldPosition(a) or GetBoneWorldPosition(b)`: a bone the skeleton lacks **raises**, so the `or` fallback never runs and the caller's bare `except` drops the entity. Use `_aim_bone_pos()` (per-name guards, then `entity.position`), which is unit-tested in `uploads/test_aim_bone.py`.
- `SkeletonESP` in `jni/ESP/ESP.h` differs from the backup on purpose: no 150 m cutoff, all 16 bone pairs at every range (the old 15/9/6 tiers dropped legs past 80 m), no per-endpoint `IsInBounds`, plus an outline pass and a filled head circle.
