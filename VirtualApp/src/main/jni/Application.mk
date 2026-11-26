# Only the 32-bit ARM ABI is built. The host app's android.ndk.abiFilters
# decides what actually gets packaged into the APK; this keeps ndk-build from
# attempting ABIs the engine has no business shipping (armeabi was removed
# from the NDK, and x86 is not a target of this launcher).
APP_ABI := armeabi-v7a
APP_PLATFORM := android-21
APP_STL := c++_static
APP_OPTIM := release
VA_ROOT          := $(call my-dir)
NDK_MODULE_PATH  := $(NDK_MODULE_PATH):$(VA_ROOT)