#pragma once

#include <android/native_activity.h>
#include <jni.h>
#include <cstdint>
#include <mutex>

// JNI_OnLoad should provide the VM. GetANativeActivity also has a safe
// JNI_GetCreatedJavaVMs fallback for constructor-based injection paths.
void SetNativeActivityJavaVM(JavaVM* vm);
void SetNativeActivity(JNIEnv* env, jobject activity);
std::recursive_mutex& GetNativeActivityMutex();
uint64_t GetNativeActivityGeneration();

extern "C" ANativeActivity* GetANativeActivity();
