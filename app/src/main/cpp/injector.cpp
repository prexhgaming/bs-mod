// ============================================================
// FILE: injector.cpp
// ============================================================
#include <jni.h>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <dlfcn.h>
#include <android/log.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>

#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "Injector", __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, "Injector", __VA_ARGS__)

extern "C" {

JNIEXPORT jint JNICALL
Java_com_nexora_InjectorActivity_findPid(JNIEnv* env, jobject thiz, jstring packageName) {
    const char* pkg = env->GetStringUTFChars(packageName, NULL);
    char pid[32];
    FILE* fp;
    int result = -1;
    char cmd[512];
    
    snprintf(cmd, sizeof(cmd), "pidof %s 2>/dev/null", pkg);
    fp = popen(cmd, "r");
    if (fp) {
        if (fgets(pid, sizeof(pid), fp)) {
            result = atoi(pid);
            if (result > 0) {
                pclose(fp);
                env->ReleaseStringUTFChars(packageName, pkg);
                return result;
            }
        }
        pclose(fp);
    }
    
    snprintf(cmd, sizeof(cmd), "ps -A 2>/dev/null | grep '%s' | awk '{print $2}' | head -1", pkg);
    fp = popen(cmd, "r");
    if (fp) {
        if (fgets(pid, sizeof(pid), fp)) {
            result = atoi(pid);
            if (result > 0) {
                pclose(fp);
                env->ReleaseStringUTFChars(packageName, pkg);
                return result;
            }
        }
        pclose(fp);
    }
    
    snprintf(cmd, sizeof(cmd), "pgrep -f '%s' 2>/dev/null | head -1", pkg);
    fp = popen(cmd, "r");
    if (fp) {
        if (fgets(pid, sizeof(pid), fp)) {
            result = atoi(pid);
            if (result > 0) {
                pclose(fp);
                env->ReleaseStringUTFChars(packageName, pkg);
                return result;
            }
        }
        pclose(fp);
    }
    
    env->ReleaseStringUTFChars(packageName, pkg);
    return -1;
}

JNIEXPORT jboolean JNICALL
Java_com_nexora_InjectorActivity_isRooted(JNIEnv* env, jobject thiz) {
    if (access("/system/bin/su", F_OK) == 0) return JNI_TRUE;
    if (access("/system/xbin/su", F_OK) == 0) return JNI_TRUE;
    if (access("/sbin/su", F_OK) == 0) return JNI_TRUE;
    if (access("/magisk/su", F_OK) == 0) return JNI_TRUE;
    if (access("/sbin/.magisk", F_OK) == 0) return JNI_TRUE;
    int ret = system("su -c 'echo test' > /dev/null 2>&1");
    return ret == 0 ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT void JNICALL
Java_com_nexora_InjectorActivity_killAntiCheat(JNIEnv* env, jobject thiz) {
    system("su -c 'setenforce 0' 2>/dev/null");
    system("su -c 'killall pharos 2>/dev/null'");
    system("su -c 'killall watchdog 2>/dev/null'");
    system("su -c 'killall anti-cheat 2>/dev/null'");
    system("su -c 'killall security 2>/dev/null'");
    system("su -c 'pm disable com.netease.pharos 2>/dev/null'");
    system("su -c 'pm disable com.netease.security 2>/dev/null'");
}

JNIEXPORT jboolean JNICALL
Java_com_nexora_InjectorActivity_copyLibraryToTmp(JNIEnv* env, jobject thiz, jstring libPath) {
    const char* lib = env->GetStringUTFChars(libPath, NULL);
    if (access(lib, F_OK) != 0) {
        env->ReleaseStringUTFChars(libPath, lib);
        return JNI_FALSE;
    }
    system("su -c 'mkdir -p /data/local/tmp' 2>/dev/null");
    system("su -c 'rm -f /data/local/tmp/system.so' 2>/dev/null");
    char cmd[512];
    snprintf(cmd, sizeof(cmd), "su -c 'cp -f \"%s\" /data/local/tmp/system.so'", lib);
    system(cmd);
    if (access("/data/local/tmp/system.so", F_OK) == 0) {
        system("su -c 'chmod 777 /data/local/tmp/system.so' 2>/dev/null");
        env->ReleaseStringUTFChars(libPath, lib);
        return JNI_TRUE;
    }
    snprintf(cmd, sizeof(cmd), "su -c 'cat \"%s\" > /data/local/tmp/system.so'", lib);
    system(cmd);
    if (access("/data/local/tmp/system.so", F_OK) == 0) {
        system("su -c 'chmod 777 /data/local/tmp/system.so' 2>/dev/null");
        env->ReleaseStringUTFChars(libPath, lib);
        return JNI_TRUE;
    }
    env->ReleaseStringUTFChars(libPath, lib);
    return JNI_FALSE;
}

JNIEXPORT jboolean JNICALL
Java_com_nexora_InjectorActivity_injectLDPreload(JNIEnv* env, jobject thiz, jint pid) {
    if (access("/data/local/tmp/system.so", F_OK) != 0) return JNI_FALSE;
    char cmd[256];
    snprintf(cmd, sizeof(cmd), "su -c 'echo \"LD_PRELOAD=/data/local/tmp/system.so\" > /proc/%d/environ' 2>/dev/null", pid);
    int result = system(cmd);
    if (result == 0) return JNI_TRUE;
    snprintf(cmd, sizeof(cmd), "su -c 'dd if=/data/local/tmp/system.so of=/proc/%d/mem bs=4096 conv=notrunc' 2>/dev/null", pid);
    result = system(cmd);
    return result == 0 ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL
Java_com_nexora_InjectorActivity_injectZygote(JNIEnv* env, jobject thiz) {
    if (access("/data/local/tmp/system.so", F_OK) != 0) return JNI_FALSE;
    FILE* fp = popen("pidof zygote 2>/dev/null || pidof zygote64 2>/dev/null", "r");
    if (!fp) return JNI_FALSE;
    char pid[32];
    if (!fgets(pid, sizeof(pid), fp)) { pclose(fp); return JNI_FALSE; }
    pclose(fp);
    char* nl = strchr(pid, '\n');
    if (nl) *nl = '\0';
    char cmd[256];
    snprintf(cmd, sizeof(cmd), "su -c 'echo \"LD_PRELOAD=/data/local/tmp/system.so\" >> /proc/%s/environ' 2>/dev/null", pid);
    int result = system(cmd);
    return result == 0 ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL
Java_com_nexora_InjectorActivity_injectPtrace(JNIEnv* env, jobject thiz, jint pid) {
    if (access("/data/local/tmp/system.so", F_OK) != 0) return JNI_FALSE;
    char cmd[256];
    snprintf(cmd, sizeof(cmd), "su -c 'cat /data/local/tmp/system.so > /proc/%d/mem' 2>/dev/null", pid);
    int result = system(cmd);
    if (result == 0) return JNI_TRUE;
    snprintf(cmd, sizeof(cmd), "su -c 'dd if=/data/local/tmp/system.so of=/proc/%d/mem bs=4096 conv=notrunc' 2>/dev/null", pid);
    result = system(cmd);
    return result == 0 ? JNI_TRUE : JNI_FALSE;
}

JNIEXPORT jboolean JNICALL
Java_com_nexora_InjectorActivity_loadFromTmp(JNIEnv* env, jobject thiz) {
    if (access("/data/local/tmp/system.so", F_OK) != 0) return JNI_FALSE;
    void* handle = dlopen("/data/local/tmp/system.so", RTLD_NOW);
    if (handle) return JNI_TRUE;
    return JNI_FALSE;
}

}
