#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <thread>
#include <android/log.h>
#include <sys/stat.h>
#include <sys/system_properties.h>
#include <dlfcn.h>
#include <android/native_activity.h>
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
#include "xhook/xhook.h"
#include "include/hpp/srcpy.h"
#include "NetworkBlocker.hpp"

char FIFO_PATH[256] = {0};

bool g_pending_cjk_reload = false;
bool g_cjkLoaded = false;
float global_anim_time = 0.f;
GLuint g_logo_tex = 0;
ANativeActivity* g_activity = nullptr;
int Width = 0, Height = 0;
int screenWidth = -1, screenHeight = -1;
std::atomic<bool> aimgui_thread_ready{false};
bool windowOpen = true;
std::unordered_map<std::string, bool> sync_bool;
std::unordered_map<std::string, float> sync_float;
std::unordered_map<std::string, int> sync_int;
ImVec2 windowPos = {80, 120};
std::atomic<bool> menu_visible{true};

// PyRun_StringFlags in libGame.so, found by find_pyrun.py (see that script for
// the method). Re-run it and change this number when the game updates.
static constexpr uintptr_t PyRunOffset = 0x024dbcf0;

extern void AddNotification(const char* msg, int r, int g, int b, float dur);

// Defined in draw.h (included further down). Called as soon as the activity -
// and with it the internal data dir - is known, so the previous run's
// data.json is gone before anything can read it.
static inline void sync_with_py_start_cleanup();

extern "C" void ANativeActivity_onCreate(ANativeActivity* activity, void* savedState, size_t savedStateSize) {
    g_activity = activity;
    sync_with_py_start_cleanup();
    LOGI("✅ Activity created, waiting for bypass thread...");
}

static int (*original_connect)(int, const struct sockaddr*, socklen_t) = nullptr;
static int my_connect(int sockfd, const struct sockaddr* addr, socklen_t addrlen) {
    return original_connect(sockfd, addr, addrlen);
}

static bool (*_hook_bypass)(void*) = nullptr;
static bool hook_bypass(void*) { return false; }

static int iLoaded = 0, iLoadAt = 2;
DEFINES(void*, iMissMyWife, const char* v0, int v1, void* v2, void* v3, void* v4) {
    static bool loaded = false;
    if (v1 == 257) {
        iLoaded++;
        if (iLoaded >= iLoadAt && !loaded) {
            loaded = true;
            _iMissMyWife(srcpy, 257, v2, v2, NULL);
        }
    }
    return _iMissMyWife(v0, v1, v2, v3, v4);
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

void hack_thread() {
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

    NetworkBlocker::initialize();
    
    xhook_register(".*\\.so$", "getaddrinfo", (void*)NetworkBlocker::getaddrinfo_hooked, (void**)&orig_getaddrinfo);
    xhook_register(".*\\.so$", "gethostbyname", (void*)NetworkBlocker::gethostbyname_hooked, (void**)&orig_getaddrinfo);
    xhook_register(".*\\.so$", "connect", (void*)my_connect, (void**)&original_connect);
    xhook_refresh(0);

    menu_visible = true;
    AddNotification("✅ NEXORA READY!", 140, 89, 242, 2.f);
    
    HOOKI((ptr)libGameBase + PyRunOffset, iMissMyWife);

    IMGUI();
    __INPUT__();
}

__attribute__((constructor))
void __0__() {
    std::thread(hack_thread).detach();
}
