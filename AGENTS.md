# Project notes

## Layout
- `:loader` (`loader/`) is the app: one screen (`LauncherActivity`), app id `zyn.bs`, and it owns the mod. `:core` (`core/`) is the renjana container engine vendored as an **Android library** (namespace `com.fesu.renjana`, no `applicationId`, no LAUNCHER entry).
- The mod's native project **is** `loader/src/main/cpp` (`Android.mk`, `Application.mk`, sources). `loader/build.gradle.kts` wires it with `externalNativeBuild { ndkBuild { path("src/main/cpp/Android.mk") } }`, so `./gradlew :loader:assembleRelease` (or `assembleDebug` for a quick local build) compiles `libzyn-bs.so` and packages it in one step. There is no separate `jni/` directory, no standalone `ndk-build` invocation, and no `.so` copied in from outside — do not reintroduce one.
- Manifest merger: `:core` must **not** set `android:theme` (or any other app-owned attribute such as `android:name`) on `<application>`. A library that declares one collides with `:loader`'s `Theme.Launcher` and the build dies with `Attribute application@theme ... is also present at [:core]`. Every `:core` activity declares its own theme instead, so dropping the application-level one changes nothing. `uploads/check_manifest_merge.py` predicts this class of conflict — run it after touching either manifest; `tools:replace` is the escape hatch, not the fix.
- ABI is 32-bit end to end: `loader/src/main/cpp/Application.mk` `APP_ABI := armeabi-v7a` and `loader/build.gradle.kts` `abiFilters += "armeabi-v7a"`. Shipping arm64-v8a lets the container pick a 64-bit process that can map neither the game's 32-bit `libGame.so` nor the mod.
- NDK: one pinned version, `ndk = "27.3.13750724"` (r27d) in `gradle/libs.versions.toml`, applied as `ndkVersion` in `loader/build.gradle.kts`, overridable with `-PndkVersion=<v>`. **Gradle installs it** — `android.builder.sdkDownload=true` in `gradle.properties` makes AGP fetch the platform, build-tools and the NDK itself, so CI has no SDK/NDK step and there is no `-PndkPath` plumbing or unpacked archive checked in. The first build after a version bump is slower while ~660 MB downloads.
- CI: `.github/workflows/build.yml` — checkout, JDK 17, recreate signing from
  `SIGNING_*` secrets, then `./gradlew :loader:assembleRelease` teed to `build.log`
  (Gradle supplies the SDK/NDK). It checks `lib/armeabi-v7a/libzyn-bs.so` is in the
  APK and deliberately does **not** assert the signature: signing is optional by
  design, so a missing keystore must yield an unsigned APK rather than a failed
  build. Telegram gets the APK on success; on failure it gets a text-only message,
  never the log, so a post-build check failing cannot arrive captioned "build
  failed". `pipefail` is what makes a failed build fail the step — without it `tee`
  masks the status.
- Build scripts are **Kotlin DSL** (`*.gradle.kts`) with every dependency and version in `gradle/libs.versions.toml`. Bump a version there, not in a module script; the catalog is loaded automatically, so `settings.gradle.kts` has no `versionCatalogs` block. `uploads/check_gradle_catalog.py` verifies every `libs.*` accessor actually exists (a typo is a configuration-time failure).
- Kotlin's own plugin ids must **not** go through the catalog. `org.jetbrains.kotlin.plugin.parcelize` is registered by kotlin-gradle-plugin, which the root script already puts on the classpath, so `:core` applies it as `id("org.jetbrains.kotlin.plugin.parcelize")` — no version, no `alias(...)`. Giving it a version fails with "plugin is already on the classpath with an unknown version, so compatibility cannot be checked". The same goes for any id contributed by KGP instead of published as its own marker.
- Two workflows: `.github/workflows/build.yml` (release, signed, Telegram, runs on push) and `debug.yml` (debug APK artifact, `workflow_dispatch` **only** so every push is not built twice). The debug one intentionally uses **no cache and no secrets** so it can never seed a stale entry into the release job's Gradle cache.
- Build settings in `gradle.properties`: `org.gradle.parallel=true` and `org.gradle.caching=true`, with `-Xmx4096m` because parallel workers overlap `:core`'s Kotlin/KSP compile with `:loader`'s ndk-build. Do not drop the heap back to 2g while parallel is on.
- Release is `isMinifyEnabled = true` + `isShrinkResources = true`. That is only safe because `:core` declares `consumerProguardFiles("proguard-rules.pro")`, which keeps the reflectively-reached engine pieces; `loader/proguard-rules.pro` covers only the loader's own classes. If a release-only crash appears, check the core rules before disabling minification.
- **Signing**: `keystore.properties` lives at the **repository root**, next to
  `settings.gradle.kts` (`storeFile` / `storePassword` / `keyAlias` / `keyPassword`).
  `loader/build.gradle.kts` loads it with `rootProject.file(...)`, so
  `storeFile=release.jks` means `<root>/release.jks` — never a module-relative
  path, which is what made an earlier CI run miss the file. The signing config is
  created only when that file exists and is then picked up **by name**
  (`signingConfigs.getByName('release')`); no env vars are consulted. Without it
  the release APK is left **unsigned** rather than debug-signed, so a mistake
  fails loudly instead of shipping a debug-key APK. Both files are gitignored; CI
  recreates them from the `SIGNING_*` secrets and deletes them in an
  `if: always()` step. Same key + same `applicationId zyn.bs` is what lets a new
  build upgrade the installed app in place; a different key makes Android refuse
  the install as a signature mismatch. Never regenerate this keystore.
- `versionCode`/`versionName` continue from the previous release (`401` /
  `4.0.1` after `400` / `4.0.0`) so upgrades are accepted rather than rejected
  as downgrades. Bump both for a real release.
- `:core` is a **library**, so AGP does not generate `BuildConfig.VERSION_NAME` /
  `VERSION_CODE` / `APPLICATION_ID` — referencing them is a compile error (the
  split hit this once in `SettingsScreen`). Read the app version at runtime via
  `Utils.appVersionName(context)`; `uploads/check_modules.py` now rejects those
  fields in `:core`.
- Do **not** install the NDK with a third-party setup action (e.g. `nttld/setup-ndk`): those unpack the archive with a JS extractor that drops symlinks, so `clang++` (a symlink to `clang`) is dangling and every compile fails with `clang++: No such file or directory`. AGP's own downloader (what we rely on) and `sdkmanager` both recreate symlinks correctly.
- CI: `.github/workflows/cleanup.yml` — weekly cron (Sun 03:00 UTC) that deletes artifacts older than 7 days, caches unused for 7 days, and all but the newest 5 runs per workflow. Manual run takes `keep_runs`, `artifact_days`, `cache_days`, `keep_ndk_cache`, `dry_run`. Only `completed` runs are deleted, so in-progress ones are never touched. The `ndk-*` cache protection is now a no-op (the NDK comes from Gradle's cache) but harmless.

## Settings plumbing (easy to break silently)
- The injected script (`loader/src/main/cpp/include/cpp/srcpy.cpp`, the `srcpy` raw string) reads **only** `data.json`. There is no UDP settings channel — do not add one back.
- C++ writes it from `loader/src/main/cpp/include/sync_with_py.h` into `GetConfigDir()` = `ANativeActivity::internalDataPath + "/Nexora/"`. Internal dir only, **no** `/storage/emulated/0` fallback: a fallback makes C++ and the script resolve different paths, so `sync_attrs` stays empty and every feature whose Python default is `False` (`bAIM`, `bESP_Skeleton`) looks broken while `bESP` (default `True`) still draws.
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
- `SkeletonESP` in `loader/src/main/cpp/ESP/ESP.h` differs from the backup on purpose: no 150 m cutoff, all 16 bone pairs at every range (the old 15/9/6 tiers dropped legs past 80 m), no per-endpoint `IsInBounds`, plus an outline pass and a filled head circle.

## Loader ↔ core boundary
- `LoaderApp` **extends** `RenjanaApplication`, so that class must stay `open`. It is what publishes `RenjanaApplication.get()`, installs `CrashHandler`, initialises Pine / `GuestInfoCache` / `DeviceFingerprint` / `GoogleSignInVirtualizer` and warms the instance caches from Room. A plain `Application` in the loader would leave all of it uninitialised.
- Never hardcode the host package. `CONTAINER_PACKAGE` in `core/.../hooks/*` resolves `RenjanaApplication.get().packageName` at use time; the old literal `com.fesu.renjana` is false for every "is this the host, not the guest?" check now that the app id belongs to the loader.
- The mod is loaded by `zyn.bs.ModLoader` (`:loader`), which registers itself through `GuestModInjection.register(...)` in `LoaderApp.onCreate`; `StubActivity` then calls `GuestModInjection.install(this, effectivePackage)` immediately after the guest `Application.onCreate()` — the same lifecycle point VirtualApp's `AppCallback.afterApplicationCreate` used. **The library name and target package live in `GameTarget` (`MOD_LIBRARY`, `PACKAGE_NAME`), not in `:core`** — `:core` must never reference `:loader` (circular dependency) and must stay fork-agnostic. Loading is gated on `GameTarget.PACKAGE_NAME` and never throws: a missing `.so` must degrade to "the game runs without the menu", not a dead guest launch.
- The launcher design (wallpaper `1795549016113.jpg`, `colors/themes/dimens`, 15 drawables, `activity_launcher.xml`) was restored from commit `4a4c256`. Only the design was reused — none of the old VirtualApp/BlackBox code came back.
- There is no Android SDK or Gradle in the agent sandbox, so the APK cannot be compiled locally; the first real build is CI.
