#include "FrameRateController.hpp"

#include <algorithm>
#include <android/log.h>
#include <cmath>
#include <jni.h>

namespace {

constexpr const char* kLogTag = "FrameRateController";

bool ClearException(JNIEnv* env) {
    if (!env || !env->ExceptionCheck()) {
        return false;
    }
    env->ExceptionClear();
    return true;
}

bool RequestDisplayMode(ANativeActivity* activity, float requestedFps) {
    if (!activity || !activity->vm || !activity->clazz) {
        return false;
    }

    JNIEnv* env = nullptr;
    bool attached = false;
    const jint state = activity->vm->GetEnv(
        reinterpret_cast<void**>(&env),
        JNI_VERSION_1_6);
    if (state == JNI_EDETACHED) {
        if (activity->vm->AttachCurrentThread(&env, nullptr) != JNI_OK) {
            return false;
        }
        attached = true;
    } else if (state != JNI_OK || !env) {
        return false;
    }

    bool success = false;
    jclass activityClass = env->GetObjectClass(activity->clazz);
    jmethodID getWindowManager = activityClass
        ? env->GetMethodID(
              activityClass,
              "getWindowManager",
              "()Landroid/view/WindowManager;")
        : nullptr;
    jobject windowManager = getWindowManager
        ? env->CallObjectMethod(activity->clazz, getWindowManager)
        : nullptr;
    if (ClearException(env)) windowManager = nullptr;

    jclass windowManagerClass = windowManager
        ? env->GetObjectClass(windowManager)
        : nullptr;
    jmethodID getDefaultDisplay = windowManagerClass
        ? env->GetMethodID(
              windowManagerClass,
              "getDefaultDisplay",
              "()Landroid/view/Display;")
        : nullptr;
    jobject display = getDefaultDisplay
        ? env->CallObjectMethod(windowManager, getDefaultDisplay)
        : nullptr;
    if (ClearException(env)) display = nullptr;

    jint bestModeId = 0;
    float bestDistance = 100000.0f;
    if (display && requestedFps > 0.0f) {
        jclass displayClass = env->GetObjectClass(display);
        jmethodID getSupportedModes = displayClass
            ? env->GetMethodID(
                  displayClass,
                  "getSupportedModes",
                  "()[Landroid/view/Display$Mode;")
            : nullptr;
        if (ClearException(env)) getSupportedModes = nullptr;

        jobjectArray modes = getSupportedModes
            ? static_cast<jobjectArray>(
                  env->CallObjectMethod(display, getSupportedModes))
            : nullptr;
        if (ClearException(env)) modes = nullptr;

        if (modes) {
            const jsize count = env->GetArrayLength(modes);
            for (jsize i = 0; i < count; ++i) {
                jobject mode = env->GetObjectArrayElement(modes, i);
                jclass modeClass = mode ? env->GetObjectClass(mode) : nullptr;
                jmethodID getRefreshRate = modeClass
                    ? env->GetMethodID(modeClass, "getRefreshRate", "()F")
                    : nullptr;
                jmethodID getModeId = modeClass
                    ? env->GetMethodID(modeClass, "getModeId", "()I")
                    : nullptr;
                if (getRefreshRate && getModeId && !ClearException(env)) {
                    const float refreshRate = env->CallFloatMethod(mode, getRefreshRate);
                    const jint modeId = env->CallIntMethod(mode, getModeId);
                    if (!ClearException(env)) {
                        const float distance = std::fabs(refreshRate - requestedFps);
                        if (distance < bestDistance) {
                            bestDistance = distance;
                            bestModeId = modeId;
                        }
                    }
                } else {
                    ClearException(env);
                }
                if (modeClass) env->DeleteLocalRef(modeClass);
                if (mode) env->DeleteLocalRef(mode);
            }
            env->DeleteLocalRef(modes);
        }
        if (displayClass) env->DeleteLocalRef(displayClass);
    }

    jmethodID getWindow = activityClass
        ? env->GetMethodID(
              activityClass,
              "getWindow",
              "()Landroid/view/Window;")
        : nullptr;
    jobject window = getWindow
        ? env->CallObjectMethod(activity->clazz, getWindow)
        : nullptr;
    if (ClearException(env)) window = nullptr;

    if (window) {
        jclass windowClass = env->GetObjectClass(window);
        jmethodID getAttributes = windowClass
            ? env->GetMethodID(
                  windowClass,
                  "getAttributes",
                  "()Landroid/view/WindowManager$LayoutParams;")
            : nullptr;
        jobject attributes = getAttributes
            ? env->CallObjectMethod(window, getAttributes)
            : nullptr;
        if (ClearException(env)) attributes = nullptr;

        if (attributes) {
            jclass attributesClass = env->GetObjectClass(attributes);
            jfieldID preferredRefreshRate = attributesClass
                ? env->GetFieldID(attributesClass, "preferredRefreshRate", "F")
                : nullptr;
            if (ClearException(env)) preferredRefreshRate = nullptr;
            if (preferredRefreshRate) {
                env->SetFloatField(attributes, preferredRefreshRate, requestedFps);
                success = !ClearException(env);
            }

            jfieldID preferredDisplayModeId = attributesClass
                ? env->GetFieldID(attributesClass, "preferredDisplayModeId", "I")
                : nullptr;
            if (ClearException(env)) preferredDisplayModeId = nullptr;
            if (preferredDisplayModeId && (bestModeId > 0 || requestedFps <= 0.0f)) {
                env->SetIntField(attributes, preferredDisplayModeId, bestModeId);
                success = !ClearException(env) || success;
            }

            jmethodID setAttributes = windowClass
                ? env->GetMethodID(
                      windowClass,
                      "setAttributes",
                      "(Landroid/view/WindowManager$LayoutParams;)V")
                : nullptr;
            if (setAttributes && !ClearException(env)) {
                env->CallVoidMethod(window, setAttributes, attributes);
                success = !ClearException(env) || success;
            } else {
                ClearException(env);
            }

            if (attributesClass) env->DeleteLocalRef(attributesClass);
            env->DeleteLocalRef(attributes);
        }

        if (windowClass) env->DeleteLocalRef(windowClass);
        env->DeleteLocalRef(window);
    }

    if (display) env->DeleteLocalRef(display);
    if (windowManagerClass) env->DeleteLocalRef(windowManagerClass);
    if (windowManager) env->DeleteLocalRef(windowManager);
    if (activityClass) env->DeleteLocalRef(activityClass);
    if (attached) activity->vm->DetachCurrentThread();
    return success;
}

} // namespace

namespace FrameRateController {

bool Apply(ANativeActivity* activity,
           EGLDisplay gameDisplay,
           bool enabled,
           float targetFps) {
    (void)gameDisplay;
    targetFps = std::max(30.0f, std::min(targetFps, 120.0f));

    // Do NOT call eglSwapInterval() on the game's EGL display here. The game
    // owns its frame pacing and may deliberately use a vendor/adaptive mode;
    // forcing interval=1 from the overlay can turn a 90/120 Hz game into a
    // 45/60/30 FPS cadence. Only request Android's display mode and let the
    // game keep its own swap policy.
    const bool displayModeApplied = RequestDisplayMode(
        activity,
        enabled ? targetFps : 0.0f);

    __android_log_print(
        ANDROID_LOG_INFO,
        kLogTag,
        "FPS display request %s, target %.0f (display=%d; game EGL untouched)",
        enabled ? "ON" : "OFF",
        targetFps,
        displayModeApplied);
    return displayModeApplied;
}

} // namespace FrameRateController
