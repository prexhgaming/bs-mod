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
- **GMS is invisible by design**: Play services are cloned by `GameRepository.provision()` (game first, then GMS) but must never appear as a chip, row, icon, menu item or status line. Provisioning stages use generic wording only (`launcher_stage_*`). The one deliberate exception is a **failed** clone: it surfaces through the existing error banner (`SandboxStatus.gmsError` → `GameActivity.renderNotice`), because a silent failure looked identical to success and only showed up as an in-game "Play services are missing".
- **Split-bundle detection trap (fixed — do not reintroduce)**: `GmsCore.installGApps` clones the *device's* Google packages through `installPackageAsUser` → `BPackageManagerService.installPackageAsUserLocked` → `isApksBundle()`. The old content fallback counted **any** two `.apk` entries anywhere in the zip, and GMS's `base.apk` embeds its dynamite modules as `.apk` files in assets — so GMS was "installed" as `com.google.android.gms.policy_ads_fdr_dynamite` (`AdsDynamite.apk`) while real `com.google.android.gms` never entered the sandbox. `isApksBundle` now treats `AndroidManifest.xml` at the archive root as proof of an ordinary APK, and requires `toc.pb`/root-level split APKs for a bundle. Recognise the symptom by `Settings: loaded Package:` listing `gsf`/`vending` but **no** `com.google.android.gms`, with the game logging `GooglePlayServicesUtil: ... they are missing`.
- `installGApps` installs GMS/GSF first (`GOOGLE_CRITICAL` via `LinkedHashSet` — it used to be an unordered `HashSet`) and only reports those two as fatal; a peripheral failure (wearables, feedback, setupwizard, or a 32-bit-only APK rejected by `AbiUtils.isSupport()` on a 64-bit box) is logged and skipped, instead of `uninstallGApps` wiping the whole set on any single failure.
- `GmsProxy: Failed to get gms service binder` is **benign** — it probes for a `"gms"` binder in the real servicemanager, which normally has none. Don't chase it when debugging GMS.
- Entry animation, breathing halos and the CTA light sweep are plain `ValueAnimator`/`ViewPropertyAnimator` (alpha/scale/translation only). Cancel them in `onDestroy`.
- The engine's `ClientConfiguration` in this SDK has no `isHideXposed()`; overriding it breaks the Kotlin build.

## Native mod menu (`libzyn-bs`)
- Sources are the uploaded `src.zip` (`main/jni/**`), dropped in **verbatim** at `app/src/main/cpp/**` → `libzyn-bs.so`. Keep the odd `小鱼字体.h` (6.7 MB CJK font, included under that raw name by `draw.h`). Only four files are *not* byte-identical to the archive: `Android.mk` + `Application.mk` (see below), `main.cpp` (the game-build hook offset — the uploaded one carries a stray `HOOKI(addr,, func)`, whose 2-arg macro silently cannot expand) and `xhook/xh_util.c` (runtime page size — see the 16 KB note below). Keep local edits to upstream files that small and listed here.
- Built with **ndk-build**, using the archive's own `Android.mk` + `Application.mk` as the **single build description** (STL, ABIs, API level, 16 KB pages, all flags live there), wired through `android.externalNativeBuild.ndkBuild` in `app/build.gradle` (top-level `android {}` block, **not** inside `buildFeatures {}`). There is **no CMakeLists.txt and it must not come back**: `arguments(...)` is not a valid method on the top-level `externalNativeBuild.cmake` block (`CmakeOptions`), and two parallel build descriptions are how source lists drift. Adding a source = editing `LOCAL_SRC_FILES` in `Android.mk` only.
- `LOCAL_MODULE := zyn-bs` is what names the output `libzyn-bs.so`; `Application.mk`'s `APP_MODULES := zyn-bs libdobby` builds **only** the menu, so the `injector` module is skipped while the prebuilt Dobby static lib is still built as a dependency.
- `injector.cpp` ships for archive fidelity but is **not compiled** (kept out via `APP_MODULES`): upstream it is a second module (`libinjector.so`) exporting `Java_com_nexora_InjectorActivity_*`, i.e. the client half of a separate ptrace-injector app. This sandbox does its own injecting.
- **Exactly one load site**: `BlackBoxLoader.afterApplicationOnCreate` (guest process, package must equal `GameTarget.PACKAGE_NAME`) → `util/ZynBs.kt` → `System.loadLibrary("zyn-bs")`, guarded by a `/proc/self/cmdline` check and a `catch (Throwable)` so a bad `.so` can never stop the game. Never load it from `App.onCreate` (host process) and never add a second site.
- Nothing crosses JNI in either direction: no `external fun`, no reliance on `JNI_OnLoad`. `main.cpp`'s `__attribute__((constructor)) void __0__()` detaches a worker thread and returns, so the game's main thread never blocks — that is also why no Java call is needed after `loadLibrary`.
- The overlay needs no NativeActivity: `AImGui/NativeActivityHelper.cpp` fabricates an `ANativeActivity` by reflecting `ActivityThread.mActivities` / `currentActivityThread`.
- Dobby ships prebuilt archives for **arm64-v8a + armeabi-v7a only** (`Dobby/libraries/<abi>/libdobby.a`), matching the app's `abiFilters`; adding an x86 ABI fails at **link** time, not compile time.
- Two flag traps worth keeping: `-D__BIONIC_NO_PAGE_SIZE_MACRO` hides libc's `PAGE_SIZE`, so hardcoded-4096 page math fails at compile time instead of corrupting memory on 16 KB devices — that trade is **deliberate**, and the answer to a resulting error is to switch the code to `sysconf(_SC_PAGE_SIZE)`, not to delete the flag. Vendored `xhook/xh_util.c` used to be the one offender (it relied on the removed `PAGE_SIZE`/`PAGE_MASK` macros); it now derives both from the runtime page size, like `KittyMemory.h`'s `_SYS_PAGE_SIZE_`, `include/includes.h` and `include/ori.h` always did. `-fpermissive -fexceptions` are required by the upstream dialect. The archive's `-latomic` is **deliberately dropped** — the NDK removed libatomic in r23, so it would only produce a "non-system library" warning and a link error. `Application.mk` asks for `APP_PLATFORM := android-24`, but AGP passes `APP_PLATFORM=android-21` (from `minSdk`) on the command line, which wins; harmless here because nothing in the sources is newer than API 9 (`__system_property_get`, `ANativeWindow_setBuffersGeometry`).
- **Pinned to one game build**: `main.cpp` waits for a stable `libGame.so` base, inline-hooks `libGameBase + 0x106520c`, and picks its process by the `newspike`/`newspikeme` names. A game update moves that offset — the menu will hook the wrong code or crash. Check the offset before blaming the loader.
- The menu stores config in `/data/data/<game-pkg>/imgui` (internal storage → **no permission**; do not add `MANAGE_EXTERNAL_STORAGE`/`WRITE_EXTERNAL_STORAGE`). `NetworkBlocker.hpp` hooks the **game's** `getaddrinfo`/`gethostbyname`/`connect`, which the game already holds `INTERNET` for.
- Engine gotcha: the sandbox redirects `SharedPreferences` into the game's own virtual data dir, so host and guest processes read *different* prefs files. Any decision shared between them must live in a file the guest can read, not in prefs.
- Removed by this swap — do not resurrect: the Python SDK dumper (`sdk_dumper.cpp`, `py_runtime_dump.cpp`, `native_utils.cpp`, `third_party/`, `util/PySdkDumper.kt`) and its More-menu toggle (`action_dump_sdk`). `uploads/` (reference material, e.g. `src.zip`) and `tmp/` (scratch rigs) are gitignored.

## Toolchain
- Gradle wrapper 8.7, AGP 8.5.2, Kotlin 1.9.23, JDK 17, `compileSdk 35`, `targetSdk 34`, `minSdk 21`, `ndkVersion 29.0.13846066`, ABIs `arm64-v8a` + `armeabi-v7a`.
- `compileSdk`/`targetSdk`/`minSdk` live in root `build.gradle` (`ext.*`). `compileSdk 37` fails — the CI runner has no `platforms;android-37`.

## Building — NOT possible in this sandbox
- The sandbox has **no JDK and no Android SDK/NDK**, so `./gradlew` cannot verify anything here — a native or Gradle mistake only surfaces on CI, so keep native-build edits conservative and push to check.
- Build on CI (`.github/workflows/release-apk.yml`: JDK 17 + `setup-android`, `assembleRelease`, Telegram notifications) or locally in Android Studio.
- Repositories are plain `google()` / `mavenCentral()` / JitPack — **no Aliyun mirrors** (they return 502 and abort resolution).

## Git
- `/workspace` is a full clone of `github.com/prexhgaming/bs-mod`; `main` tracks `origin/main` and the working tree must stay identical to it (`.base/` and `.polydev/` are gitignored app bookkeeping).
- Remote is plain HTTPS with **no token in the repo config** — auth comes from the global `gh` CLI credential helper (logged in as `prexhgaming`). Commits here author as `prexhgaming <prexhgaming@users.noreply.github.com>`.
- `core.filemode=true`: the sandbox import can drop the executable bit, so `gradlew` must stay mode `100755` (`chmod +x gradlew`) or every `git status` shows a bogus mode change.
