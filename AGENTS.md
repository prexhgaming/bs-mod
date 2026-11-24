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
- `blackbox-gallery-stub/` — upstream helper app, **not** in `settings.gradle` and unused here.

## Launcher UI (`:app`)
- One screen, one game, one action. The old multi-app grid/list is gone — do not reintroduce an app list or multi-app selection.
- Full-bleed wallpaper is `app/src/main/assets/1795549016113.jpg`, decoded at runtime in `GameActivity.decodeWallpaper()` (downsampled to screen width, RGB_565) and darkened by `bg_hero_scrim` + `bg_hero_vignette`.
- **GMS is invisible by design**: Play services are still cloned by `GameRepository.provision()` (game first, then GMS) but must never appear as a chip, row, icon, menu item or status line. Provisioning stages use generic wording only (`launcher_stage_*`).
- Entry animation, breathing halos and the CTA light sweep are plain `ValueAnimator`/`ViewPropertyAnimator` (alpha/scale/translation only). Cancel them in `onDestroy`.
- The engine's `ClientConfiguration` in this SDK has no `isHideXposed()`; overriding it breaks the Kotlin build.

## Python SDK dumper
- Target game is a **CPython 3.11.2** title whose interpreter is statically linked into **`libGame.so`** (the splits ship no standalone `libpython*.so`). Its `Resources.mpk` is a **zstd-compressed tar** (`237MPTZSTD1` magic + `28 B5 2F FD` frame).
- Native: `app/src/main/cpp/sdk_dumper.cpp` → `libbs_dumper.so`, built by `app/src/main/cpp/CMakeLists.txt` via `android.externalNativeBuild.cmake` in `app/build.gradle` (top-level `android {}` block, **not** inside `buildFeatures {}`).
- Bridge: `app/.../util/PySdkDumper.kt`. Only JNI entry point is `Java_top_niunaijun_blackboxa_util_PySdkDumper_nativeStart(String outDir)` (member `external fun` in a Kotlin `object` ⇒ `jobject`, not `jclass`). Java passes **only** the Downloads path — everything else is discovered natively.
- Toggle: checkable "More" menu item `action_dump_sdk`, persisted in SharedPreferences `bs_dumper_prefs` / `sdk_dumper_enabled`. The toggle only arms the dumper; the dump runs on the **next game launch**. Output goes to `/storage/emulated/0/Download/<game-pkg>/sdk` (`env.json`, `maps.txt`, `python/`, `libs/`).
- **Trigger**: `JNI_OnLoad` spawns a worker thread that waits for the output dir; `nativeStart()` unblocks it. The lib is loaded from the **guest lifecycle hook** (`BlackBoxLoader.afterApplicationOnCreate`, package must equal `GameTarget.PACKAGE_NAME`) — not from `App.onCreate`, which runs in the *host* process and therefore only ever sees the launcher's own dirs.
- Native discovery (no dirs passed from Java): parse `/proc/self/maps` for `<sandbox-root>/data/app/<pkg>/base.apk` → `appDir`, `libDir = appDir/lib`, then `dataDir = <sandbox-root>/data/user/<uid>/<pkg>`. Sandbox root is `<host-data>/blackbox` (`BEnvironment`).
- Python is found by scanning **every** `.so` in `libDir` in 8 MB chunks for the `Py_GetVersion` banner (`3.x.y (`) — do **not** cap the scan (`libGame.so` is ~84 MB and the scanner is prefix-anchored, so a cap silently misses it).
- Downloads writes: API 23-29 use `WRITE_EXTERNAL_STORAGE` (`maxSdkVersion="29"`) plus `requestLegacyExternalStorage`; API 30+ use `MANAGE_EXTERNAL_STORAGE` ("All files access"), checked via `Environment.isExternalStorageManager()`. Ticking the dumper checkbox deep-links to `ACTION_MANAGE_APP_ALL_FILES_ACCESS_PERMISSION` (there is no runtime dialog); without the grant the dump falls back to app-private storage.
- Uploaded game splits (`split_config.*.apk`, `Resources.mpk`) are gitignored — reference material, not project source.

## Toolchain
- Gradle wrapper 8.7, AGP 8.5.2, Kotlin 1.9.23, JDK 17, `compileSdk 35`, `targetSdk 34`, `minSdk 21`, `ndkVersion 29.0.13846066`, ABIs `arm64-v8a` + `armeabi-v7a`.
- `compileSdk`/`targetSdk`/`minSdk` live in root `build.gradle` (`ext.*`). `compileSdk 37` fails — the CI runner has no `platforms;android-37`.

## Building — NOT possible in this sandbox
- The sandbox has **no JDK and no Android SDK/NDK**, so `./gradlew` cannot verify anything here; check XML/resource/view-binding consistency with a script instead.
- Build on CI (`.github/workflows/release-apk.yml`: JDK 17 + `setup-android`, `assembleRelease`, Telegram notifications) or locally in Android Studio.
- Repositories are plain `google()` / `mavenCentral()` / JitPack — **no Aliyun mirrors** (they return 502 and abort resolution).
