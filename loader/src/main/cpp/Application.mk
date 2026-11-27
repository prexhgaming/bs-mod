# 32-bit only. The target game's libGame.so is armeabi-v7a, and a 64-bit process
# can never load a 32-bit library: shipping arm64-v8a would let the sandbox pick a
# 64-bit process that cannot map the game at all. Building just this ABI keeps the
# APK 32-bit, so the game's process is 32-bit too.
# (Trade-off: 64-bit-only devices, e.g. recent Pixels, cannot install this APK.
#  Add arm64-v8a back here and in app/build.gradle's abiFilters if that matters.)
APP_ABI := armeabi-v7a
APP_STL       := c++_static

# Only the mod menu is built. The `injector` module defined in Android.mk is
# deliberately left out: upstream it is a separate ptrace-injector app (its JNI
# entry points are Java_com_nexora_InjectorActivity_*), and this sandbox does
# its own injecting. libdobby is the prebuilt static dependency, listed so it is
# built as well.
APP_MODULES   := zyn-bs libdobby
APP_OPTIM     := release
APP_THIN_ARCHIVE := true
APP_PIE       := true

# Explicit target API level (was left to the NDK default before).
APP_PLATFORM  := android-24

# ==================== 16KB PAGE SIZE FIX (Android 15/16) ====================
# Official ndk-build flag so every object we compile is aligned to 16KB instead
# of relying on manual LOCAL_LDFLAGS. Needs NDK r26+ (older NDKs silently ignore
# it -- check <ndk_root>/source.properties -> Pkg.Revision).
APP_SUPPORT_FLEXIBLE_PAGE_SIZES := true

# Hide libc's PAGE_SIZE macro so any hardcoded `PAGE_SIZE` / `4096` page-align
# math fails at COMPILE time instead of silently crashing at runtime on 16KB
# page devices. If this triggers an error, that code must use getpagesize() /
# sysconf(_SC_PAGESIZE).
APP_CPPFLAGS  += -D__BIONIC_NO_PAGE_SIZE_MACRO
APP_CFLAGS    += -D__BIONIC_NO_PAGE_SIZE_MACRO

# ==================== 32-BIT TARGET ====================
# Only armeabi-v7a is built: the game is a 32-bit library and a 64-bit process
# cannot load it. If arm64-v8a is ever re-enabled, restore it in APP_ABI as well
# as in app/build.gradle's abiFilters.
