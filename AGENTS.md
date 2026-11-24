# Project notes

## What this is
- **NewBlackbox / BlackBox virtual engine** — Android app-cloning / sandboxing SDK + demo app.
- Source: `github.com/BotBotHack/NewBlackboxSRC` (branch `main`), cloned into the project root. No `.git` from upstream was imported (the app's own `.git/` metadata is kept).

## Modules
- `:app` — Kotlin demo UI (`top.niunaijun.blackboxa`), launcher `WelcomeActivity`, `Application` = `.app.App`.
- `:Bcore` — core engine: Java (`top.niunaijun.blackbox.BlackBoxCore`), 55 AIDL files (`Bcore/src/main/aidl`), and native C++ (`Bcore/src/main/cpp`, `Android.mk` + `Application.mk`). No prebuilt `.so` are committed — native code is compiled from source by NDK.

## Toolchain
- Gradle wrapper 8.7, AGP 8.5.2, Kotlin 1.9.23, JDK 17, `compileSdk 35`, `targetSdk 34`, `minSdk 21`, `ndkVersion 29.0.13846066`, ABIs `arm64-v8a` + `armeabi-v7a`.
- `minSdk` lives in root `build.gradle` (`ext.minSdk = 21`); README's "API 24" is outdated — trust the gradle files.

## Building — NOT possible in this sandbox
- The sandbox has **no JDK and no Android SDK/NDK** (intentionally not installed), so `./gradlew` and `check_preview` cannot verify anything here.
- Build on CI (`.github/workflows/build_and_telegram.yml`: JDK 17 + `setup-android`, runs `assembleDebug`, `assembleRelease`, `:Bcore:assembleRelease`) or locally in Android Studio.
- Local commands once a JDK 17 + Android SDK exist: `./gradlew assembleDebug`, `./gradlew :Bcore:assembleRelease`.
- Dependencies resolve from JitPack + Aliyun mirrors + google/mavenCentral (`settings.gradle`, `FAIL_ON_PROJECT_REPOS`).

## Docs in repo
- `Docs.md` — user guide + API reference; `README.md` — overview/features; `*_FIX.md` at root — per-issue fix notes from upstream.
