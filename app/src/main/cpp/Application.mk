APP_ABI       := arm64-v8a armeabi-v7a
APP_STL       := c++_static
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

# ==================== 32 + 64 BIT ====================
# Both ABIs build from the same sources:
#   arm64-v8a  -> Dobby/libraries/arm64-v8a/libdobby.a
#   armeabi-v7a-> Dobby/libraries/armeabi-v7a/libdobby.a
# AndInlineHook/AndInlineHook.h picks the right one at compile time, so hook
# call sites are identical on 32-bit and 64-bit.
