#include "NativeActivityHelper.hpp"

#include <android/log.h>
#include <chrono>
#include <cstring>
#include <dlfcn.h>
#include <mutex>

namespace {

constexpr const char* kLogTag = "NativeActivityHelper";
std::recursive_mutex g_activityMutex;
JavaVM* g_javaVM = nullptr;
ANativeActivity g_nativeActivity{};
jobject g_activityGlobalRef = nullptr;
uint64_t g_activityGeneration = 0;
std::chrono::steady_clock::time_point g_lastAttempt{};

bool ClearException(JNIEnv* env) {
    if (!env || !env->ExceptionCheck()) {
        return false;
    }
    env->ExceptionClear();
    return true;
}

JavaVM* ResolveJavaVM() {
    if (g_javaVM) {
        return g_javaVM;
    }

    using GetCreatedVMsFn = jint (*)(JavaVM**, jsize, jsize*);
    auto getCreatedVMs = reinterpret_cast<GetCreatedVMsFn>(
        dlsym(RTLD_DEFAULT, "JNI_GetCreatedJavaVMs"));
    if (!getCreatedVMs) {
        return nullptr;
    }

    JavaVM* vm = nullptr;
    jsize count = 0;
    if (getCreatedVMs(&vm, 1, &count) == JNI_OK && count > 0) {
        g_javaVM = vm;
    }
    return g_javaVM;
}

jobject FindCurrentActivity(JNIEnv* env) {
    jclass activityThreadClass = env->FindClass("android/app/ActivityThread");
    if (!activityThreadClass || ClearException(env)) {
        return nullptr;
    }

    jmethodID currentActivityThread = env->GetStaticMethodID(
        activityThreadClass,
        "currentActivityThread",
        "()Landroid/app/ActivityThread;");
    if (!currentActivityThread || ClearException(env)) {
        env->DeleteLocalRef(activityThreadClass);
        return nullptr;
    }

    jobject activityThread = env->CallStaticObjectMethod(
        activityThreadClass,
        currentActivityThread);
    if (!activityThread || ClearException(env)) {
        env->DeleteLocalRef(activityThreadClass);
        return nullptr;
    }

    jfieldID activitiesField = env->GetFieldID(
        activityThreadClass,
        "mActivities",
        "Landroid/util/ArrayMap;");
    if (!activitiesField || ClearException(env)) {
        env->DeleteLocalRef(activityThread);
        env->DeleteLocalRef(activityThreadClass);
        return nullptr;
    }

    jobject activities = env->GetObjectField(activityThread, activitiesField);
    if (!activities || ClearException(env)) {
        env->DeleteLocalRef(activityThread);
        env->DeleteLocalRef(activityThreadClass);
        return nullptr;
    }

    jclass mapClass = env->GetObjectClass(activities);
    jmethodID valuesMethod = mapClass
        ? env->GetMethodID(mapClass, "values", "()Ljava/util/Collection;")
        : nullptr;
    jobject values = valuesMethod
        ? env->CallObjectMethod(activities, valuesMethod)
        : nullptr;
    if (!values || ClearException(env)) {
        if (mapClass) env->DeleteLocalRef(mapClass);
        env->DeleteLocalRef(activities);
        env->DeleteLocalRef(activityThread);
        env->DeleteLocalRef(activityThreadClass);
        return nullptr;
    }

    jclass collectionClass = env->GetObjectClass(values);
    jmethodID iteratorMethod = collectionClass
        ? env->GetMethodID(collectionClass, "iterator", "()Ljava/util/Iterator;")
        : nullptr;
    jobject iterator = iteratorMethod
        ? env->CallObjectMethod(values, iteratorMethod)
        : nullptr;

    jobject result = nullptr;
    if (iterator && !ClearException(env)) {
        jclass iteratorClass = env->GetObjectClass(iterator);
        jmethodID hasNext = iteratorClass
            ? env->GetMethodID(iteratorClass, "hasNext", "()Z")
            : nullptr;
        jmethodID next = iteratorClass
            ? env->GetMethodID(iteratorClass, "next", "()Ljava/lang/Object;")
            : nullptr;

        while (hasNext && next && env->CallBooleanMethod(iterator, hasNext)) {
            jobject record = env->CallObjectMethod(iterator, next);
            if (!record || ClearException(env)) {
                if (record) env->DeleteLocalRef(record);
                continue;
            }

            jclass recordClass = env->GetObjectClass(record);
            jfieldID activityField = recordClass
                ? env->GetFieldID(recordClass, "activity", "Landroid/app/Activity;")
                : nullptr;
            if (activityField && !ClearException(env)) {
                result = env->GetObjectField(record, activityField);
            } else {
                ClearException(env);
            }

            if (recordClass) env->DeleteLocalRef(recordClass);
            env->DeleteLocalRef(record);
            if (result) {
                break;
            }
        }

        if (iteratorClass) env->DeleteLocalRef(iteratorClass);
    }

    if (iterator) env->DeleteLocalRef(iterator);
    if (collectionClass) env->DeleteLocalRef(collectionClass);
    env->DeleteLocalRef(values);
    if (mapClass) env->DeleteLocalRef(mapClass);
    env->DeleteLocalRef(activities);
    env->DeleteLocalRef(activityThread);
    env->DeleteLocalRef(activityThreadClass);
    return result;
}

} // namespace

void SetNativeActivityJavaVM(JavaVM* vm) {
    if (!vm) {
        return;
    }
    std::lock_guard<std::recursive_mutex> lock(g_activityMutex);
    g_javaVM = vm;
}

void SetNativeActivity(JNIEnv* env, jobject activity) {
    if (!env || !activity) {
        return;
    }

    std::lock_guard<std::recursive_mutex> lock(g_activityMutex);
    JavaVM* vm = g_javaVM;
    if (!vm && env->GetJavaVM(&vm) == JNI_OK) {
        g_javaVM = vm;
    }
    if (!vm) {
        return;
    }

    if (g_activityGlobalRef && env->IsSameObject(g_activityGlobalRef, activity)) {
        return;
    }

    jobject newActivityRef = env->NewGlobalRef(activity);
    if (!newActivityRef || ClearException(env)) {
        return;
    }
    if (g_activityGlobalRef) {
        env->DeleteGlobalRef(g_activityGlobalRef);
    }

    g_activityGlobalRef = newActivityRef;
    std::memset(&g_nativeActivity, 0, sizeof(g_nativeActivity));
    g_nativeActivity.vm = vm;
    g_nativeActivity.env = env;
    g_nativeActivity.clazz = g_activityGlobalRef;
    ++g_activityGeneration;
    __android_log_print(
        ANDROID_LOG_INFO,
        kLogTag,
        "Host Activity updated from onResume");
}

extern "C" ANativeActivity* GetANativeActivity() {
    std::lock_guard<std::recursive_mutex> lock(g_activityMutex);
    if (g_activityGlobalRef) {
        return &g_nativeActivity;
    }

    const auto now = std::chrono::steady_clock::now();
    if (g_lastAttempt.time_since_epoch().count() != 0 &&
        now - g_lastAttempt < std::chrono::milliseconds(500)) {
        return nullptr;
    }
    g_lastAttempt = now;

    JavaVM* vm = ResolveJavaVM();
    if (!vm) {
        return nullptr;
    }

    JNIEnv* env = nullptr;
    const jint envState = vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6);
    if (envState == JNI_EDETACHED) {
        if (vm->AttachCurrentThread(&env, nullptr) != JNI_OK) {
            return nullptr;
        }
    } else if (envState != JNI_OK || !env) {
        return nullptr;
    }

    jobject activity = FindCurrentActivity(env);
    if (!activity) {
        return nullptr;
    }

    g_activityGlobalRef = env->NewGlobalRef(activity);
    env->DeleteLocalRef(activity);
    if (!g_activityGlobalRef || ClearException(env)) {
        g_activityGlobalRef = nullptr;
        return nullptr;
    }

    std::memset(&g_nativeActivity, 0, sizeof(g_nativeActivity));
    g_nativeActivity.vm = vm;
    g_nativeActivity.env = env;
    g_nativeActivity.clazz = g_activityGlobalRef;
    ++g_activityGeneration;

    __android_log_print(
        ANDROID_LOG_INFO,
        kLogTag,
        "Active Activity resolved for secure overlay");
    return &g_nativeActivity;
}

std::recursive_mutex& GetNativeActivityMutex() {
    return g_activityMutex;
}

uint64_t GetNativeActivityGeneration() {
    return g_activityGeneration;
}
