LOCAL_PATH := $(call my-dir)

# ============================================================
# PREBUILT STATIC LIBRARIES
# ============================================================
# Dobby is the single hooking engine for BOTH ABIs (32-bit and 64-bit).
include $(CLEAR_VARS)
LOCAL_MODULE    := libdobby
LOCAL_SRC_FILES := Dobby/libraries/$(TARGET_ARCH_ABI)/libdobby.a
include $(PREBUILT_STATIC_LIBRARY)

# ============================================================
# MAIN MODULE
# ============================================================
include $(CLEAR_VARS)

# Output name: libzyn-bs.so (matches System.loadLibrary("zyn-bs")).
LOCAL_MODULE := zyn-bs

LOCAL_CFLAGS := -w -s -Wno-error=format-security -fvisibility=hidden -fpermissive -fexceptions
LOCAL_CPPFLAGS := -w -s -Wno-error=format-security -fvisibility=hidden -std=c++17 -fpermissive -Wall -fexceptions
LOCAL_CPPFLAGS += -Wno-error=c++11-narrowing -fstack-protector-strong -frtti -O3
LOCAL_CPPFLAGS += -fdata-sections -ffunction-sections -fvisibility-inlines-hidden -fno-omit-frame-pointer
LOCAL_CFLAGS += -fstack-protector-strong -O3 -fdata-sections -ffunction-sections
LOCAL_CFLAGS += -fvisibility-inlines-hidden -fno-omit-frame-pointer

LOCAL_LDFLAGS += -Wl,--gc-sections,--strip-all,-llog
# No -latomic: the NDK dropped libatomic in r23 (r23+ / this project's r29 would
# fail at link pointing at a library that no longer ships). Atomics resolve
# through libc++ / compiler-rt, and only lock-free bool atomics are used here.
LOCAL_LDLIBS := -llog -landroid -lEGL -lGLESv3 -lz -ldl

# 32-bit only: build our own code in ARM mode (arm64 has no such mode).
ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)
LOCAL_ARM_MODE := arm
endif

# ── Source Files ──────────────────────────────────────────────
LOCAL_SRC_FILES := \
    main.cpp \
    KittyMemory/KittyMemory.cpp \
    KittyMemory/MemoryPatch.cpp \
    KittyMemory/MemoryBackup.cpp \
    KittyMemory/KittyUtils.cpp \
    KittyMemory/KittyScanner.cpp \
    include/cpp/imgui.cpp \
    include/cpp/dlfcn.cpp \
    include/cpp/srcpy.cpp \
    include/cpp/imgui/custom_theme.cpp \
    xhook/xhook.c \
    xhook/xh_core.c \
    xhook/xh_elf.c \
    xhook/xh_jni.c \
    xhook/xh_log.c \
    xhook/xh_util.c \
    xhook/xh_version.c \
    Oxorany/oxorany.cpp

# ── Include Paths ──────────────────────────────────────────────
LOCAL_C_INCLUDES := \
    $(LOCAL_PATH) \
    $(LOCAL_PATH)/AndInlineHook \
    $(LOCAL_PATH)/include \
    $(LOCAL_PATH)/include/cpp \
    $(LOCAL_PATH)/include/cpp/imgui \
    $(LOCAL_PATH)/imgui \
    $(LOCAL_PATH)/imgui/backends \
    $(LOCAL_PATH)/KittyMemory \
    $(LOCAL_PATH)/xhook \
    $(LOCAL_PATH)/Dobby

LOCAL_STATIC_LIBRARIES := libdobby

include $(BUILD_SHARED_LIBRARY)
