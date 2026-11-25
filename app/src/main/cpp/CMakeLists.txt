# ---------------------------------------------------------------------------
# libzyn-bs.so — the in-game mod menu (ImGui overlay + hooks).
#
# THIS FILE IS AUTHORITATIVE. Gradle builds only this file
# (android.externalNativeBuild.cmake in app/build.gradle). The Android.mk /
# Application.mk next to it are the upstream ndk-build description, kept as the
# reference they were ported from — they are never executed. Every source file,
# include dir, compile flag and linker flag below mirrors them; if you add a
# source, add it in both places or the two descriptions drift apart.
#
# The port maps 1:1 with one exception: ndk-build's `APP_PLATFORM := android-24`
# is NOT forced here. The ndk-build project was built for API 24, but a scan of
# the sources finds no symbol newer than API 9 (__system_property_get,
# ANativeWindow_setBuffersGeometry, EGL/GLES3), so there is nothing to gain from
# pinning the native platform above the app's minSdk — AGP drives it from
# minSdkVersion instead. Revisit only if a compile error makes it necessary.
#
# ABI note: Dobby ships prebuilt archives for armeabi-v7a and arm64-v8a only,
# which is exactly the abiFilters list in app/build.gradle. Adding an x86 ABI
# fails the link, not the C++ compile.
# ---------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.22.1)

project("zyn-bs")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(NATIVE_SRC ${CMAKE_CURRENT_SOURCE_DIR})

# ---------------------------------------------------------------------------
# Dobby — Apache-2.0, github.com/jmpews/Dobby
# The single hooking engine for both ABIs; AndInlineHook/AndInlineHook.h picks
# the right entry points at compile time, so hook call sites are identical on
# 32- and 64-bit.
# ---------------------------------------------------------------------------
add_library(dobby STATIC IMPORTED)
set_target_properties(dobby PROPERTIES
    IMPORTED_LOCATION ${NATIVE_SRC}/Dobby/libraries/${ANDROID_ABI}/libdobby.a
)
target_include_directories(dobby INTERFACE ${NATIVE_SRC}/Dobby)

# ---------------------------------------------------------------------------
# The mod menu. Named `zyn-bs` -> libzyn-bs.so, which is what the Kotlin side
# loads (top.niunaijun.blackboxa.util.ZynBs) inside the game's process only.
#
# injector.cpp is deliberately NOT in this list: in the archive it is a second,
# standalone module (libinjector.so) whose JNI entry points are
# Java_com_nexora_InjectorActivity_*, i.e. the client half of a separate
# out-of-process ptrace injector app. The sandbox does the injecting itself, so
# building it here would only ship a dead library with JNI exports for an
# Activity this APK does not have.
# ---------------------------------------------------------------------------
add_library(zyn-bs SHARED
    main.cpp
    AImGui/AImGui.cpp
    AImGui/NativeActivityHelper.cpp
    AImGui/FrameRateController.cpp
    KittyMemory/KittyMemory.cpp
    KittyMemory/MemoryPatch.cpp
    KittyMemory/MemoryBackup.cpp
    KittyMemory/KittyUtils.cpp
    KittyMemory/KittyScanner.cpp
    include/cpp/imgui.cpp
    include/cpp/dlfcn.cpp
    include/cpp/srcpy.cpp
    include/cpp/imgui/custom_theme.cpp
    xhook/xhook.c
    xhook/xh_core.c
    xhook/xh_elf.c
    xhook/xh_jni.c
    xhook/xh_log.c
    xhook/xh_util.c
    xhook/xh_version.c
    Oxorany/oxorany.cpp
)

# `-D__BIONIC_NO_PAGE_SIZE_MACRO` hides libc's PAGE_SIZE macro, so hardcoded
# 4096 page-align math fails at COMPILE time instead of silently corrupting
# memory on 16 KB page devices (Android 15/16). Combined with
# -DANDROID_SUPPORT_FLEXIBLE_PAGE_SIZES=ON (passed from app/build.gradle) every
# object and segment we emit is 16 KB aligned.
set(ZYN_SHARED_FLAGS
    -O3
    -fvisibility=hidden
    -fvisibility-inlines-hidden
    -fstack-protector-strong
    -fdata-sections
    -ffunction-sections
    -fno-omit-frame-pointer
    -D__BIONIC_NO_PAGE_SIZE_MACRO
)

# -w then -Wall, in the archive's order. -fpermissive / -fexceptions / -frtti
# are what the upstream code relies on to compile at all (old-style casts and
# throwing through C++ frames in C-callback-heavy hook code).
set(ZYN_C_FLAGS   -w -Wno-error=format-security -fexceptions -fpermissive ${ZYN_SHARED_FLAGS})
set(ZYN_CXX_FLAGS ${ZYN_C_FLAGS} -std=c++17 -Wall -Wno-error=c++11-narrowing -frtti)

target_compile_options(zyn-bs PRIVATE
    $<$<COMPILE_LANGUAGE:C>:${ZYN_C_FLAGS}>
    $<$<COMPILE_LANGUAGE:CXX>:${ZYN_CXX_FLAGS}>
)

# 32-bit only: build our own code in ARM mode (arm64 has no such mode), matching
# ndk-build's LOCAL_ARM_MODE := arm.
if(ANDROID_ABI STREQUAL "armeabi-v7a")
    target_compile_options(zyn-bs PRIVATE -marm)
endif()

target_include_directories(zyn-bs PRIVATE
    ${NATIVE_SRC}
    ${NATIVE_SRC}/AndInlineHook
    ${NATIVE_SRC}/AImGui
    ${NATIVE_SRC}/include
    ${NATIVE_SRC}/include/cpp
    ${NATIVE_SRC}/include/cpp/imgui
    ${NATIVE_SRC}/imgui
    ${NATIVE_SRC}/imgui/backends
    ${NATIVE_SRC}/KittyMemory
    ${NATIVE_SRC}/xhook
    ${NATIVE_SRC}/Dobby
)

find_library(log-lib log)
find_library(android-lib android)
find_library(egl-lib EGL)
find_library(gles-lib GLESv3)
find_library(z-lib z)
find_library(dl-lib dl)
find_library(atomic-lib atomic)

target_link_options(zyn-bs PRIVATE -Wl,--gc-sections,--strip-all)

target_link_libraries(zyn-bs
    dobby
    ${log-lib}
    ${android-lib}
    ${egl-lib}
    ${gles-lib}
    ${z-lib}
    ${dl-lib}
    ${atomic-lib}
)
