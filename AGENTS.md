# Project notes

## What this is
- **BS-KILLER** — a single-game launcher built on the **VirtualApp virtual engine** (Android app cloning / sandboxing).
- Engine: `com.lody.virtual.client.core.VirtualCore` from the **VirtualApp** engine (`github.com/ServenScorpion/VirtualApp`), **built from source** as the `:VirtualApp` module. The prebuilt `app/sdk/lib-release.aar` is kept only as a reference artifact and is *not* on the compile classpath.
- The old `:Bcore` engine (and its `Bcore/` directory) has been deleted entirely.
- The target game is hard-wired in `app/src/main/java/zyn/bs/GameTarget.kt` (`com.netease.newspike`, "Knives Out", sandbox user 0).

## Modules
- `:app` — Kotlin launcher UI (`zyn.bs`). Launcher activity `.view.game.GameActivity`, `Application` `.app.App`. Also hosts the native mod menu `libzyn-bs.so`.
- `:VirtualApp` — the engine, vendored source (`com.lody.virtual.**` + `com.xdja.**`, 657 Java, 86 AIDL, native `src/main/jni`). `namespace com.lody.virtual`, manifest `package=` removed (AGP 8). Depends only on `androidx.core`; appcompat/support-v7 were dropped and the 16 `android.support.*` imports rewritten to AndroidX.
- `:app` and `:VirtualApp` are the **only** modules. The Bcore-era `:black-reflection` + `:compiler` (annotation-processor pair) were deleted: nothing imported `zyn.bs.core.reflection.**` and nothing declared `annotationProcessor`/`kapt` for them.
- Engine wiring: `App.onCreate` calls `VirtualCore.get().startup(this, SettingConfig)` and registers `AppCallback`; `AppManager`/`BlackBoxLoader`/`GameRepository` use `VirtualCore`. **No `BlackBoxCore`.**

### Engine build constraints (`:VirtualApp`)
- **ABI ownership**: `APP_ABI := armeabi-v7a` in `VirtualApp/src/main/jni/Application.mk` (NDK 29 dropped `armeabi`); the *packaging* filter stays in `:app` (`android.defaultConfig.ndk.abiFilters`). 64-bit is not a target — the game is 32-bit only.
- `BuildConfig.VERSION_CODE` is not generated for libraries under AGP 8, so the module declares `VA_VERSION_NAME` / `VA_VERSION_CODE` / `commit` itself and `com.lody.virtual.Build` reads those.
- `manifestPlaceholders` `PERMISSION_PREFIX` / `AUTHORITY_PREFIX` = `zyn.bs`; they must match `:app`'s `applicationId` or the engine's providers collide.
- **Dead vendor SDK removed**: the jcenter-only `com.xdja.safekeyservice:*` artifacts (unavailable, jcenter is shut) were only used by `VSafekeyManagerService`, so that service, its client, the CKMS variant and 3 AIDLs were deleted along with its `BinderProvider` registration. `IVSCallback.aidl` is kept — `VirtualCore.VSCallback` extends its `Stub`.
- `app/proguard-rules.pro` keeps the engine wholesale (`-keep class com.lody.virtual.**`): it resolves stub components, AIDL stubs and hook proxies by name, so renaming breaks the sandbox at runtime.

## VirtualApp integration
- **Lifecycle**: `VirtualCore.get().setAppCallback(AppCallback)`; `AppCallback.afterApplicationCreate(packageName, processName, application)` fires in the guest process after `callApplicationOnCreate`. **libzyn-bs is mapped there** (`ZynBs.load(packageName, USER_ID)`, guarded by `catch(Throwable)`); the host process never loads it. No `AppLifecycleCallback`/`BActivityThread`.
- **Engine init**: `VirtualCore.startup(Context, SettingConfig)` on the main thread; `waitForEngine()` for async readiness. `SettingConfig` requires `getHostPackageName()` + `getPluginEnginePackageName()`.
- **GMS**: **ignored** (user decision). No GMS clone; `SandboxStatus.gmsSupported=false`, `gmsInstalled=true`, `gmsError=null`, no GMS stage. VirtualApp does not have BlackBox's Google-services problem.
- **Permissions**: **not requested in VirtualApp** (user decision); the game handles its own. `:app` still declares `RECORD_AUDIO`/`POST_NOTIFICATIONS` for the sandboxed game's uid; no runtime request from the launcher. No `MANAGE_EXTERNAL_STORAGE`.
- **Install**: `VirtualCore.installPackageAsUser(int userId, String pkg)` (arg order differs from Bcore); APK-from-URI uses `installPackage(apkPath, InstallOptions)`. **Launch**: `VActivityManager.get().launchApp(userId, pkg)` (not on `VirtualCore`). **Uninstall**: `uninstallPackageAsUser(pkg, userId)`. **Clear data**: `cleanPackageData(pkg, userId)`. **Installed**: `isAppInstalled(pkg)` / `getPackageManager()`. **Query**: `getPackageManager().getInstalledApplications(0)`.
- **Sandbox prefs**: VirtualApp redirects `SharedPreferences` into the game's virtual data dir; host and guest read different prefs. Share via files, not prefs.

## Launcher UI (`:app`)
- One screen, one game, one action. Do not add an app list, app grid or multi-app selection.
- Full-bleed wallpaper `app/src/main/assets/1795549016113.jpg`, decoded in `GameActivity` (downsampled to screen width, RGB_565), darkened by `bg_hero_scrim` + `bg_hero_vignette`.
- Provisioning clones the game only; no GMS stage. Stages use generic `launcher_stage_*` strings. A failed game import surfaces through the error banner (`SandboxStatus.message` → `renderNotice`).
- Entry animation, breathing halos and the CTA light sweep are `ValueAnimator`/`ViewPropertyAnimator` (alpha/scale/translation only), cancelled in `onDestroy`.

## Native mod menu (`libzyn-bs`)
- Sources in `app/src/main/cpp`; output `libzyn-bs.so` (`LOCAL_MODULE := zyn-bs`).
- Built with **ndk-build**, driven by the archive's own `Android.mk` + `Application.mk` as the **single build description**, wired through `android.externalNativeBuild.ndkBuild` in `app/build.gradle` (top-level `android {}`, not inside `buildFeatures {}`). There is no `CMakeLists.txt` and it should not come back — two parallel build descriptions is how source lists drift. Adding a source means editing `LOCAL_SRC_FILES` in `Android.mk` only.
- `Application.mk`'s `APP_MODULES := zyn-bs libdobby` builds only the menu plus the prebuilt Dobby static lib. `injector.cpp` ships for archive fidelity but is **not compiled** (upstream it is a separate ptrace-injector app; this sandbox does its own injecting).
- **Exactly one load site**: `App.onCreate` registers a `VirtualApp AppCallback`; its `afterApplicationCreate` (guest process, game package only) → `util/ZynBs.kt` → `System.loadLibrary("zyn-bs")`, guarded by a `/proc/self/cmdline` check and `catch (Throwable)` so a bad `.so` can never stop the game. Never load from the host process, never add a second site.
- Nothing crosses JNI: no `external fun`, no reliance on `JNI_OnLoad`. `main.cpp`'s `__attribute__((constructor)) void __0__()` detaches a worker thread and returns, so the game's main thread never blocks.
- The overlay owns **no** EGL context and **no** SurfaceControl layer: `Draw()` is hooked onto `eglSwapBuffers` and paints ImGui into the game's own back buffer on the game's render thread. `g_activity` comes from the game's own `ANativeActivity_onCreate`.
- **ImGui has one global context that can be null at any time.** `CleanupGL()` leaves the current context null, and the input hooks run on the game's own input thread. Every foreign-thread ImGui entry point must pass `nx::imguiInputSafe()` (`include/hpp/imgui/ctx_guard.h`) first — `initializeMotionEvent`, `AInputQueue_getEvent`, `TrackImGuiWindow`/`IsImGuiTouchPoint`. Never cache the context pointer.
- Python hook: `PyRunOffset = 0x024dbcf0` in `main.cpp` (Thumb, verified against the shipped `libGame.so`). On a game update, re-derive it with `python3 app/src/main/cpp/find_pyrun.py libGame.so --top 5` — it scans `.text` for `movw r1,#0x101` seeds and keeps candidates where `r2 == r3` (callers pass `globals == locals`).
- The injected script `include/cpp/srcpy.cpp` is a monkey-patching mod (~34 `@HOOK` decorators on live game classes) that imports the game's own `gclient.*`/`gshare.*` framework. A private/embedded CPython is not a substitute — it would have none of those modules.
- UI → script config is a file, not a socket: `sync_with_py.h` writes a flat `{key: value}` snapshot to `data.json` in the game's internal data dir, and the injected Python re-reads it on a 100 ms loop (mtime check). **`data.json` and `config.json` are different files — do not merge them**: `config.json` is `SaveConfig()`/`LoadConfig()`'s nested `{"bool":{},"float":{},"int":{}}` document that must survive a restart; `data.json` is the throwaway flat snapshot. Writing the flat format over the nested one wipes the user's saved settings.
- C++ treats a missing `screen_w`/`screen_h` as "unknown" (no rescale, 1:1 draw) rather than assuming a resolution, so a broken config degrades to correct positions instead of a stretched overlay.
- Menu tabs, in order: **Dashboard, Aimbot, Visuals, Weapon, Settings** (`navLabels`/`navCount` in `MENU.h`). There is no Memory tab and no Skins/Hero Skins/Protection tab; player-memory mods are not part of the menu.
- The menu stores its config in the game's internal data dir — **no storage permission** is needed, and `MANAGE_EXTERNAL_STORAGE`/`WRITE_EXTERNAL_STORAGE` must not be added. `NetworkBlocker.hpp` hooks the game's own `getaddrinfo`/`gethostbyname`/`connect`, which already hold `INTERNET`.
- `SDK/` (under `app/src/main/cpp`) has been **removed** — it was a partly-corrupt offset dump, never a build input (nothing in `Android.mk` or the sources included it). Do not restore it. `.gitignore` still carries `/app/src/main/cpp/SDK` so a stray copy can never be committed.

## Toolchain
- Gradle wrapper 8.13, AGP 8.13.2, Kotlin 1.9.23, JDK 17, `compileSdk 35`, `targetSdk 28`, `minSdk 21`, `ndkVersion 29.0.13846066`, ABI `armeabi-v7a`.
- **The wrapper and AGP move together**: AGP 8.13 refuses to apply on Gradle < 8.13, so bumping `agp` in `gradle/libs.versions.toml` without `distributionUrl` in `gradle/wrapper/gradle-wrapper.properties` fails at configuration time.
- **Java target stays 17** (`sourceCompatibility`/`targetCompatibility`/`jvmTarget`): CI installs JDK 17, so a 21 target fails with `invalid target release: 21`.
- **32-bit only on purpose**: the game is an `armeabi-v7a` library and a 64-bit process cannot load it, so shipping `arm64-v8a` would let the sandbox spawn a process the game can never map into. `abiFilters` in `app/build.gradle` and `APP_ABI` in `Application.mk` must change together. Trade-off: 64-bit-only devices cannot install the APK.
- Dobby ships prebuilt archives for `arm64-v8a` + `armeabi-v7a` only; adding another ABI fails at **link** time.
- `-D__BIONIC_NO_PAGE_SIZE_MACRO` hides libc's `PAGE_SIZE` so hardcoded 4096 page math fails at compile time instead of corrupting memory on 16 KB-page devices. The answer to a resulting error is `sysconf(_SC_PAGE_SIZE)`, not deleting the flag.
- Repositories are plain `google()` / `mavenCentral()` / JitPack — no Aliyun mirrors (they return 502).

## Building
- **Not possible in this sandbox**: no JDK and no Android SDK/NDK, so `./gradlew` verifies nothing here. Keep native and Gradle edits conservative and let CI check them.
- Build on CI (`.github/workflows/release-apk.yml`: JDK 17 + `setup-android`, `assembleRelease`, Telegram notification) or locally in Android Studio.
- `release` uses `minifyEnabled true` + `shrinkResources true`. Safe because nothing resolves resources by name; the engine's reflection needs are covered by `proguard-rules.pro`.
- There is **no custom crash handler** in either `:app` or the engine — fatal exceptions must propagate normally. Don't add one back: swallowing uncaught exceptions leaves the Looper dead and turns a crash into an ANR.

## Git & CI
- `/workspace` is a **shallow** clone of `github.com/prexhgaming/bs-mod` (`--depth=1`, one commit, no tags), so `git log`/`git blame`/`HEAD~1` only see the tip; `git fetch --unshallow` restores history if an older commit is ever needed.
- If a `git` command reports "another git process seems to be running", it is a stale `.git/*.lock` — delete the lock file, don't re-clone.
- Remote is plain HTTPS with no token in the repo config; auth comes from the global `gh` CLI credential helper. Commits author as `prexhgaming <prexhgaming@users.noreply.github.com>`.
- `core.filemode=true`: `gradlew` must stay mode `100755` or every `git status` shows a bogus mode change.
- The workflow runs on **every push**, builds the release APK and posts the result (or `build.log`) to Telegram via `TELEGRAM_BOT_TOKEN`/`TELEGRAM_CHAT_ID`. Those are repo secrets — never read or print them.
- `push.py` is an optional helper that generates a commit message from the diff via an AI endpoint; it is not part of the build.
