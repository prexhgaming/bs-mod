# Project notes

## What this is
- **BS-KILLER** — a single-game launcher built on the **BlackBox virtual engine** (Android app cloning / sandboxing SDK).
- Engine source: `github.com/Black00Z/Blacks-BlackBox`. `Bcore/`, `black-reflection/` and `compiler/` are vendored from it; `app/` is this project's own code.
- Target game is hard-wired in `app/.../GameTarget.kt` (`com.netease.newspike`, "Knives Out", sandbox user 0).

## Modules

- `:app` — Kotlin UI (`top.niunaijun.blackboxa`). Launcher activity = `.view.game.GameActivity`, `Application` = `.app.App`.
- `:Bcore` — engine: Java (`top.niunaijun.blackbox.BlackBoxCore`), AIDL (`Bcore/src/main/aidl`), native C++ (`Bcore/src/main/cpp`). No prebuilt `.so` — compiled from source by the NDK.
- `:black-reflection` — annotation/reflection stubs required by `:Bcore`.
- `:compiler` — annotation processor for `:black-reflection`.

## Launcher UI (`:app`)
- One screen, one game, one action. The old multi-app grid/list is gone — do not reintroduce an app list or multi-app selection.
- Full-bleed wallpaper is `app/src/main/assets/1795549016113.jpg`, decoded at runtime in `GameActivity.decodeWallpaper()` (downsampled to screen width, RGB_565) and darkened by `bg_hero_scrim` + `bg_hero_vignette`.
- **GMS is invisible by design**: Play services are still cloned by `GameRepository.provision()` (game first, then GMS) but must never appear as a chip, row, icon, menu item or status line. Provisioning stages use generic wording only (`launcher_stage_*`).
- Entry animation, breathing halos and the CTA light sweep are plain `ValueAnimator`/`ViewPropertyAnimator` (alpha/scale/translation only). Cancel them in `onDestroy`.
- The engine's `ClientConfiguration` in this SDK has no `isHideXposed()`; overriding it breaks the Kotlin build.

## Python SDK dumper
- Target game is a **CPython 3.11.2** title whose interpreter is statically linked into **`libGame.so`** (the splits ship no standalone `libpython*.so`). Its `Resources.mpk` is a **zstd-compressed tar** (`237MPTZSTD1` magic + `28 B5 2F FD` frame).
- Native: `app/src/main/cpp/sdk_dumper.cpp` → `libbs_dumper.so`, built by `app/src/main/cpp/CMakeLists.txt` via `android.externalNativeBuild.cmake` in `app/build.gradle` (top-level `android {}` block, **not** inside `buildFeatures {}`).
- Bridge: `app/.../util/PySdkDumper.kt`. There are **no `external fun` declarations** and **no `JNI_OnLoad`** — nothing crosses JNI. The worker starts from `__attribute__((constructor)) void lib_main()` in `sdk_dumper.cpp`, which only runs `std::thread(RunDump).detach();` and returns, so the game's main thread never blocks.
- Because a constructor can run before namespace-scope C++ objects in the same DSO are constructed, **all mutable global state is a function-local static** (Meyers singleton): `LogMutex()`/`LogBuffer()` in `sdk_dumper.cpp`, `Mutex()`/`Bases()`/`Patches()` in `native_utils.cpp`. Never reintroduce a bare namespace-scope `std::string`/`std::mutex`/`std::map` there.
- `RunDump` polls `/proc/self/maps` every 250 ms for up to 90 s until the game's libs are mapped — `libGame.so` is ~84 MB and appears *after* our constructor runs.
- **Who decides**: the host. `GameRepository.launch()` calls `PySdkDumper.prepareLaunch(...)` *before* `launchApk()`, which writes `sdk_dumper.arm` into the game's virtual data dir (or deletes it when the toggle is off). The guest lifecycle hook (`BlackBoxLoader.afterApplicationOnCreate`, package must equal `GameTarget.PACKAGE_NAME`) then calls `PySdkDumper.loadIfArmed(...)` and only loads the lib if that file exists. Never read the toggle from the guest process: the engine redirects `SharedPreferences` into the game's sandbox, so the guest sees a *different* prefs file (this was the original "nothing dumps" bug).
- The lib is loaded from the guest lifecycle, not `App.onCreate` — the latter runs in the *host* process and only ever sees the launcher's own dirs.
- Output: `/storage/emulated/0/Android/data/<game-pkg>/sdk/` (`env.json`, `maps.txt`, `dump.log`, `libs/`, `python/`). App-specific external storage needs **no permission** — do not reintroduce `MANAGE_EXTERNAL_STORAGE`/`WRITE_EXTERNAL_STORAGE`. Fallback order is the host's own `Android/data/<host>/files/sdk/<pkg>`, then `/data/user/0/<host>/files/sdk/<pkg>`.
- Native discovery: parse `/proc/self/maps` for `data/app/<pkg>-<hash>==/…` (the split-install dir — the trailing `==` is what makes the package name parseable, the `-` starts the hash) → package; `…/lib/<abi>` → the real `libDir` the loader mapped from; `<sandbox-root>` (`<host-data>/blackbox`) → `dataDir = data/user/<uid>/<pkg>`.
- Python is found by scanning **every** `.so` in `libDir` in 8 MB chunks for the `Py_GetVersion` banner (`3.x.y (`) — do **not** cap the scan (`libGame.so` is ~84 MB and the scanner is prefix-anchored, so a cap silently misses it).
- Uploaded game splits (`split_config.*.apk`, `Resources.mpk`) are gitignored — reference material, not project source.

## Native third-party (`app/src/main/cpp/third_party`)
Vendored from the LGL mod-menu archive, trimmed to what actually builds:
- `xdl/` — MIT, hexhacking xDL. Enhanced `dlopen`/`dlsym`/`dl_iterate_phdr`; used by `native_utils.cpp` to resolve module bases and symbols.
- `dobby/` — Apache-2.0, prebuilt `libdobby.a` for **arm64-v8a + armeabi-v7a only** (matches the app's `abiFilters`; adding an x86 ABI needs x86 archives or the CMake link fails).
- `kitty/` — MIT, KittyMemory. Compiled with **`kNO_KEYSTONE`**: the upstream `MemoryPatch::createWithAsm` path is the only Keystone user and those archives are ~18 MB, so it stays disabled. `MemoryPatch`, `KittyScanner`, `KittyMemory` all work.
- `common/obfuscate.h` — string obfuscation macro (header only).
- Skipped on purpose: the archive's `Includes/Utils.*` and `Includes/Macros.h` — they define globals in headers (multiple-definition errors) and pull in Keystone/KittyMemory transitively. `native_utils.hpp/.cpp` replaces them with a declared-once API: `libraryBase`, `symbolAddress`, `resolveAddress`, `hookFunction`/`hookModule`, `patchBytes`/`restorePatch`, `readBytes`.

## Toolchain
- Gradle wrapper 8.7, AGP 8.5.2, Kotlin 1.9.23, JDK 17, `compileSdk 35`, `targetSdk 34`, `minSdk 21`, `ndkVersion 29.0.13846066`, ABIs `arm64-v8a` + `armeabi-v7a`.
- `compileSdk`/`targetSdk`/`minSdk` live in root `build.gradle` (`ext.*`). `compileSdk 37` fails — the CI runner has no `platforms;android-37`.

## Building — NOT possible in this sandbox
- The sandbox has **no JDK and no Android SDK/NDK**, so `./gradlew` cannot verify anything here; check XML/resource/view-binding consistency with a script instead.
- Build on CI (`.github/workflows/release-apk.yml`: JDK 17 + `setup-android`, `assembleRelease`, Telegram notifications) or locally in Android Studio.
- Repositories are plain `google()` / `mavenCentral()` / JitPack — **no Aliyun mirrors** (they return 502 and abort resolution).
