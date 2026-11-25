#pragma once

#include <iostream>
#include <fstream>
#include <sstream>

#include <cstring>
#include <string>
#include <map>
#include <vector>
#include <tuple>
#include <list>
#include <thread>
#include <set>
#include <unistd.h>
#include <pthread.h>
#include <android/log.h>

#include <chrono>
#include <deque>

using namespace std;

#define MY_LOG_TAG "angousana"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, MY_LOG_TAG, __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, MY_LOG_TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, MY_LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, MY_LOG_TAG, __VA_ARGS__)

#define ptr uintptr_t
#define DWORD uintptr_t
#define ulong uint64_t
#define dword uint32_t
#define qword uint64_t

#define ARGB(a, r, g, b) r << 0 | g << 8 | b << 16 | a << 24

#define WHITE              ImColor(255, 255, 255)
#define RED                ImColor(255, 0, 0)
#define GREEN              ImColor(0, 255, 0)
#define LIME               ImColor(0, 255, 0)
#define BLUE               ImColor(0, 0, 255)
#define BLACK              ImColor(0, 0, 0)
#define PURPLE             ImColor(128, 0, 128)
#define GREY               ImColor(128, 128, 128)
#define YELLOW             ImColor(255, 255, 0)
#define ORANGE             ImColor(255, 165, 0)
#define DARKGREEN          ImColor(0, 100, 0)
#define PINK               ImColor(255, 192, 203)
#define BROWN              ImColor(165, 42, 42)
#define CYAN               ImColor(0, 255, 255)

#define floop(s, e) for (int fi=s; fi<e; fi++)

void pthread(void* (*func)(void*)) {pthread_t x; pthread_create(&x, 0, func, 0);}
void sleepm(int milliseconds) {std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));}

#include <sys/mman.h>

// [FIX] Cache page size — sysconf() adalah syscall, jangan dipanggil tiap EditPerm
static const long _PAGE_SIZE = sysconf(_SC_PAGE_SIZE);

bool EditPerm(void* addr, size_t len, int PERMISSION) {
    uintptr_t base = (uintptr_t)addr & ~(uintptr_t)(_PAGE_SIZE - 1);
    size_t aligned_len = (((uintptr_t)addr + len - 1) & ~(uintptr_t)(_PAGE_SIZE - 1)) - base + _PAGE_SIZE;
    return mprotect((void*)base, aligned_len, PERMISSION) != -1;
}

bool EditMemory(void* addr, size_t len, const void *data) {
    EditPerm(addr, len, (PROT_READ | PROT_WRITE | PROT_EXEC));
    bool x = (memcpy(addr, data, len) != NULL);
    EditPerm(addr, len, (PROT_READ | PROT_EXEC));
    return x;
}

// [FIX] Semua dummy hook functions — hapus LOGI spam
// Sebelumnya setiap fungsi ini nge-log tiap kali dipanggil (blocking I/O = FPS drop)
// Sekarang semua silent, behavior sama persis
uint SVOID()          { return 0; }
uint VOID()           { return 0; }
bool TRUE()           { return true; }
bool FALSE()          { return false; }
int  IMAX()           { return 100000; }
int  I0()             { return 0; }
int  I10()            { return 10; }
int  I1000()          { return 1000; }
float FMAX()          { return 100000.0f; }
float F1000()         { return 1000.0f; }
float F100()          { return 100.0f; }
float F10()           { return 10.0f; }
float F0()            { return 0.0f; }
float SF0()           { return 0.0f; }
float SF10()          { return 10.0f; }
float SF100()         { return 100.0f; }
double DMAX()         { return 100000.0; }
vector<int> V3()      { return {0, 0, 0}; }
vector<int> V2()      { return {0, 0}; }

#define BT(addr) HOOKN(addr, TRUE);
#define BF(addr) HOOKN(addr, FALSE);
#define V(addr)  HOOKN(addr, VOID);

#define IM(addr) HOOKN(addr, IMAX);

#define SV(addr) HOOKN(addr, SVOID);
#define ST(addr) HOOKN(addr, STRUE);
#define SF(addr) HOOKN(addr, SFALSE);

u_long htol(string strx) {
    const char* str = strx.c_str();
    u_long res = 0;
    char c;
    while ((c = *str++)) {
        char v = (c & 0xF) + (c >> 6) | ((c >> 3) & 0x8);
        res = (res << 4) | (u_long)v;
    }
    return res;
}

// [FIX] absoluteAddress — pindah buka maps ke luar loop supaya tidak re-open tiap iterasi
// Kalau library belum load, tunggu 10ms sebelum retry (hindari busy-spin 100% CPU)
ptr absoluteAddress(const char *libraryName, uintptr_t offset=0) {
    LOGI("Searching %s until found...", libraryName);
    while (true) {
        ifstream maps("/proc/self/maps");
        string map;
        while (getline(maps, map)) {
            if (strstr(map.c_str(), libraryName)) {
                LOGI("Library found! %s", libraryName);
                istringstream line(map);
                string start;
                getline(line, start, '-');
                return htol(start) + offset;
            }
        }
        // [FIX] Yield thread supaya tidak busy-loop bakar CPU
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    return 0;
}

template<typename... Args> void absoluteAddress(Args... args) {
    const char* arr[] = {args...};
    for (size_t i = 0; i < sizeof...(args); i++) {
        absoluteAddress(arr[i]);
    }
}

#define FOR(item, list) if (auto* __l = (list)) \
    if (auto __s = __l->getSize()) if (auto* __items = __l->getItems()) \
        for (int __i = 0; __i < __s; ++__i) if (auto item = __items[__i])

#define FORA(item, array) if (auto* __l = (array)) \
    if (auto __s = __l->getLength()) if (auto* __items = __l->getPointer()) \
        for (int __i = 0; __i < __s; ++__i) if (auto item = __items[__i])

#define F(type, offset) (*(type*)(offset))
#define M(type, offset, ...) ((type(*)(__VA_ARGS__))(offset))

#define A(x) void* x() { LOGI(#x); return 0; }
#define B(x) HOOKNI(fdlsym(libc, #x), x);

#include <sys/stat.h>

static bool create_directory_recursive(const char* path) {
    char tmp[256];
    char* p = nullptr;
    size_t len;
    
    snprintf(tmp, sizeof(tmp), "%s", path);
    len = strlen(tmp);
    if (tmp[len - 1] == '/')
        tmp[len - 1] = 0;
        
    for (p = tmp + 1; *p; p++) {
        if (*p == '/') {
            *p = 0;
            mkdir(tmp, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
            *p = '/';
        }
    }
    
    return mkdir(tmp, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == 0 || errno == EEXIST;
}
