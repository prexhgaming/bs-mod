#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <android/log.h>
#include <sys/stat.h>
#include <sys/system_properties.h>
#include <dlfcn.h>
#include <android/native_activity.h>
#include <atomic>
#include <errno.h>
#include <GLES3/gl3.h>
#include "imgui/imgui.h"
#define LOGD(...)
#define LOGE(...)
#define LOGI(...)
#include "include/obfuscate.h"

static const char* PACKAGE_NAME = [] {
static char buf[128]={};
FILE* f=fopen("/proc/self/cmdline","rb");
if(f){fread(buf,1,127,f);fclose(f);}
for(int i=0;i<127;i++) if(buf[i]=='\0') buf[i]=' ';
return strstr(buf,"newspikeme") ? "com.netease.newspikeme" : "com.netease.newspike";
}();

#include "include/includes.h"
#include "AndInlineHook/AndInlineHook.h"
#include "KittyMemory/KittyMemory.h"
#include "KittyMemory/MemoryPatch.h"
#include "KittyMemory/KittyScanner.h"
#include "KittyMemory/KittyUtils.h"
#include "include/hpp/pyscan.h"
#include "xhook/xhook.h"
#include "include/hpp/srcpy.h"
#include "NetworkBlocker.hpp"
#include "SkinInfoSync.h"

char FIFO_PATH[256] = {0};
JavaVM* jvm = nullptr;

bool g_pending_cjk_reload = false;
bool g_cjkLoaded = false;
float global_anim_time = 0.f;
GLuint g_logo_tex = 0;
ANativeActivity* g_activity = nullptr;
int Width = 0, Height = 0;
int screenWidth = -1, screenHeight = -1;
std::atomic<bool> HIDEESP{false};
std::atomic<bool> aimgui_thread_ready{false};
bool g_use_aimgui_skip = false;
bool windowOpen = true;
std::unordered_map<std::string, bool> sync_bool;
std::unordered_map<std::string, float> sync_float;
std::unordered_map<std::string, int> sync_int;
ImVec2 windowPos = {80, 120};
std::atomic<bool> menu_visible{true};

extern void AddNotification(const char* msg, int r, int g, int b, float dur);

JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved) {
    jvm = vm;
    JNIEnv* env = nullptr;
    if (vm->GetEnv((void**)&env, JNI_VERSION_1_6) == JNI_OK) {
        jclass atClass = env->FindClass("android/app/ActivityThread");
        if (atClass && !env->ExceptionCheck()) {
            jmethodID currentActivityThread = env->GetStaticMethodID(atClass,
                "currentActivityThread", "()Landroid/app/ActivityThread;");
            if (currentActivityThread && !env->ExceptionCheck()) {
                jobject thread = env->CallStaticObjectMethod(atClass, currentActivityThread);
                if (thread && !env->ExceptionCheck()) {
                    jmethodID getApp = env->GetMethodID(atClass,
                        "getApplication", "()Landroid/app/Application;");
                    if (getApp && !env->ExceptionCheck()) {
                        jobject app = env->CallObjectMethod(thread, getApp);
                        if (app && !env->ExceptionCheck()) env->DeleteLocalRef(app);
                    }
                    env->DeleteLocalRef(thread);
                }
            }
            env->DeleteLocalRef(atClass);
        }
        if (env->ExceptionCheck()) env->ExceptionClear();
    }
    return JNI_VERSION_1_6;
}

extern "C" void ANativeActivity_onCreate(ANativeActivity* activity, void* savedState, size_t savedStateSize) {
    g_activity = activity;
    if (activity && activity->vm) jvm = activity->vm;
    LOGI("✅ Activity created, waiting for bypass thread...");
}

static int (*original_connect)(int, const struct sockaddr*, socklen_t) = nullptr;
static int my_connect(int sockfd, const struct sockaddr* addr, socklen_t addrlen) {
    return original_connect(sockfd, addr, addrlen);
}

static bool (*_hook_bypass)(void*) = nullptr;
static bool hook_bypass(void*) { return false; }

// Verified against the shipped libGame.so: PyRun_StringFlags lives at 0x024dbcf0
// (Thumb, 0x64 bytes). Set back to 0 to fall back to the runtime scan.
static constexpr uintptr_t kNexoraPyRunOffset = 0x024dbcf0;

// ---- Python injection: probe first, then load once the framework answers ----
// The game's own framework (gclient.*) only becomes importable after its
// bootstrap has run, and that moment differs per launch. Injecting srcpy too
// early aborts on ImportError, and the old one-shot code set "loaded" *before*
// knowing the injection worked -- so a single early failure was permanent and
// silent. Now a self-contained probe (which never raises) reports readiness by
// writing a marker file, and the real script is injected only once it appears.
// Backoff = calls 2,4,8,...,1024 so a slow bootstrap is still caught without
// hammering the interpreter.
static char g_probe[512];
static char g_marker[256];
static const char* g_probeSrc = nullptr;
static std::atomic<int> g_pyCalls{0};
static std::atomic<int> g_pyStage{0};        // 0 probing, 1 ready, 2 injected

static bool pyDue(int n) {
    for (int t = 2, i = 0; i < 10; i++, t <<= 1)
        if (n == t) return true;
    return false;
}

static void pyInitPaths() {
    if (g_probeSrc) return;
    snprintf(g_marker, sizeof(g_marker), "/data/data/%s/imgui/.nx_ready", PACKAGE_NAME);
    snprintf(g_probe, sizeof(g_probe),
             "try:\n"
             "    import gclient.framework.entities.space\n"
             "    __import__('builtins').open(r'%s','w').write('1')\n"
             "except BaseException:\n"
             "    pass\n",
             g_marker);
    g_probeSrc = g_probe;
}

static bool pyMarkerExists() {
    pyInitPaths();
    return access(g_marker, F_OK) == 0;
}

static std::atomic<int> g_pyBadCalls{0};
static std::atomic<int> g_pyValidCalls{0};

DEFINES(void*, iMissMyWife, const char* v0, int v1, void* v2, void* v3, void* v4) {
    // Self-check. Only PyRun_StringFlags receives a start code in w1
    // (256 single / 257 file / 258 eval) plus a source string in x0. If we never
    // see that, the scan picked the wrong address and you should be told so
    // instead of wondering why nothing happens.
    if (v0 && v1 >= 256 && v1 <= 258) {
        if (g_pyValidCalls.fetch_add(1) == 0)
            NX_LOGI("hook confirmed: start=%d src=%.80s", v1, v0);
    } else if (g_pyBadCalls.fetch_add(1) == 63 && g_pyValidCalls.load() == 0) {
        NX_LOGE("hook target looks WRONG: 64 calls, none with a valid start code");
    }

    if (v1 == 257 && v2) {
        pyInitPaths();
        const int n = g_pyCalls.fetch_add(1) + 1;
        int stage = g_pyStage.load(std::memory_order_acquire);

        if (stage == 0) {
            if (pyMarkerExists()) {
                stage = 1;
                g_pyStage.store(1, std::memory_order_release);
                NX_LOGI("python framework ready at call %d", n);
            } else if (pyDue(n)) {
                NX_LOGI("python probe at call %d", n);
                _iMissMyWife(g_probeSrc, 257, v2, v2, NULL);
            }
        }

        if (stage == 1) {
            g_pyStage.store(2, std::memory_order_release);
            void* r = _iMissMyWife(srcpy, 257, v2, v2, NULL);
            if (r) {
                unlink(g_marker);
                NX_LOGI("srcpy loaded at call %d", n);
            } else {
                // Imports blew up after the probe said ready: reopen the state
                // machine so a later call retries instead of giving up forever.
                g_pyStage.store(0, std::memory_order_release);
                unlink(g_marker);
                NX_LOGE("srcpy failed at call %d, will retry", n);
            }
        }
    }

    return _iMissMyWife(v0, v1, v2, v3, v4);
}

// Runs off the render path: the scan walks the whole .text of libGame.so, which
// must not hold up the overlay. The game keeps running scripts, so hooking a
// second late still catches every later call.
static void* py_hook_thread(void* arg) {
    const uintptr_t libGameBase = (uintptr_t)arg;

    // A verified offset beats the scan: it is instant, and the two scan strategies
    // do not work on this build (nothing in .text references the "<string>"
    // literal directly -- it is reached through a GOT slot for _PyRuntime -- and
    // the 257 call-site vote is too scattered to converge). The scan stays as the
    // fallback for a build without a known offset.
    uintptr_t pyRun = 0;
    if (kNexoraPyRunOffset != 0) {
        pyRun = libGameBase + kNexoraPyRunOffset;
        NX_LOGI("using PyRun_StringFlags offset +0x%llx -> %p",
                (unsigned long long)kNexoraPyRunOffset, (void*)pyRun);
    } else {
        pyRun = nx::findPyRunStringFlags("libGame.so");
    }
    if (pyRun) {
        HOOKI(pyRun, iMissMyWife);
        AddNotification("PY HOOK OK", 140, 220, 120, 2.f);
    } else {
        // Never hook a guess: entering an arbitrary function with the PyRun
        // signature is a guaranteed crash. Report and keep the rest working.
        AddNotification("PY HOOK NOT FOUND", 230, 90, 90, 3.f);
    }
    return nullptr;
}

#include "draw.h"
#include "include/input.h"

static void nexora_mkdir_recursive(const char* path) {
    char tmp[256]; char* p = NULL; size_t len;
    snprintf(tmp, sizeof(tmp), "%s", path); len = strlen(tmp);
    if (len == 0) return;
    if (tmp[len - 1] == '/') tmp[len - 1] = 0;
    for (p = tmp + 1; *p; p++) {
        if (*p == '/') { *p = 0; mkdir(tmp, 0777); *p = '/'; }
    }
    mkdir(tmp, 0777);
}

void* bypass_thread(void*) {
    ProcMap gameLib;
    do { sleep(1); gameLib = KittyMemory::getLibraryBaseMap("libGame.so"); } while (!gameLib.isValid());
    uintptr_t stableBase = (uintptr_t)gameLib.startAddress;
    int stableCount = 0;
    while (stableCount < 3) {
        usleep(500000);
        ProcMap recheck = KittyMemory::getLibraryBaseMap("libGame.so");
        if (recheck.isValid() && (uintptr_t)recheck.startAddress == stableBase) {
            stableCount++;
        } else if (recheck.isValid()) {
            stableBase = (uintptr_t)recheck.startAddress;
            stableCount = 0;
        }
    }
    sleep(1);
    uintptr_t libGameBase = stableBase;
    
    char _pkg_dir[128];
    snprintf(_pkg_dir, sizeof(_pkg_dir), "/data/data/%s/imgui", PACKAGE_NAME);
    nexora_mkdir_recursive(_pkg_dir);

    SkinInfoSync::Init();
    NetworkBlocker::initialize();
    
    xhook_register(".*\\.so$", "getaddrinfo", (void*)NetworkBlocker::getaddrinfo_hooked, (void**)&orig_getaddrinfo);
    xhook_register(".*\\.so$", "gethostbyname", (void*)NetworkBlocker::gethostbyname_hooked, (void**)&orig_getaddrinfo);
    xhook_register(".*\\.so$", "connect", (void*)my_connect, (void**)&original_connect);
    xhook_refresh(0);

    menu_visible = true;
    AddNotification("✅ NEXORA READY!", 140, 89, 242, 2.f);

    IMGUI();
    __INPUT__();

    pthread_t pyThread;
    if (pthread_create(&pyThread, nullptr, py_hook_thread, (void*)libGameBase) == 0)
        pthread_detach(pyThread);
    return nullptr;
}

__attribute__((constructor))
void __0__() {
    pthread_t thread;
    pthread_create(&thread, nullptr, bypass_thread, nullptr);
    pthread_detach(thread);
}
