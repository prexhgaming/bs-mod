#include "native_utils.hpp"

#include <android/log.h>
#include <dlfcn.h>
#include <unistd.h>

#include <cctype>
#include <cerrno>
#include <chrono>
#include <cstdarg>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <map>
#include <mutex>
#include <set>

#include "dobby/dobby.h"
#include "kitty/MemoryPatch.hpp"
#include "xdl/xdl.h"

#define TAG "BsDumper"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)

namespace bsu {
namespace {

// Function-local statics (Meyers singletons) on purpose: this library is
// started by a `__attribute__((constructor))`, and the order between such a
// constructor and namespace-scope C++ objects in the same DSO is unspecified.
// Building them on first use removes that hazard entirely.
std::mutex& Mutex() {
    static std::mutex m;
    return m;
}

// Base addresses are cached: resolving a module is not free and the answer
// never changes for the lifetime of the process.
std::map<std::string, uintptr_t>& Bases() {
    static std::map<std::string, uintptr_t> m;
    return m;
}

// Patches we created, so restorePatch() can undo them.
std::map<uintptr_t, MemoryPatch>& Patches() {
    static std::map<uintptr_t, MemoryPatch> m;
    return m;
}

uintptr_t resolveBaseLocked(const std::string& module) {
    auto& bases = Bases();
    const auto cached = bases.find(module);
    if (cached != bases.end()) return cached->second;

    void* handle = xdl_open(module.c_str(), XDL_DEFAULT);
    if (handle == nullptr) {
        // xdl_open also accepts a bare soname; try the basename as a fallback.
        const size_t slash = module.rfind('/');
        if (slash != std::string::npos) {
            handle = xdl_open(module.c_str() + slash + 1, XDL_DEFAULT);
        }
    }
    if (handle == nullptr) return 0;

    xdl_info_t info;
    memset(&info, 0, sizeof(info));
    uintptr_t base = 0;
    if (xdl_info(handle, XDL_DI_DLINFO, &info) == 0) {
        base = reinterpret_cast<uintptr_t>(info.dli_fbase);
    }
    xdl_close(handle);

    if (base != 0) bases[module] = base;
    return base;
}

// "0x1234" / "1234" -> offset, 0 when the string is not hexadecimal.
uintptr_t parseHexOffset(const std::string& value) {
    if (value.empty()) return 0;
    const char* start = value.c_str();
    if (value.size() > 2 && start[0] == '0' && (start[1] == 'x' || start[1] == 'X')) {
        start += 2;
    }
    if (*start == '\0') return 0;

    for (const char* c = start; *c != '\0'; ++c) {
        if (!isxdigit(static_cast<unsigned char>(*c))) return 0;
    }
    return static_cast<uintptr_t>(strtoull(start, nullptr, 16));
}

}  // namespace

// ---------------------------------------------------------------- modules

std::vector<std::string> loadedModules() {
    std::vector<std::string> modules;
    FILE* fp = fopen("/proc/self/maps", "re");
    if (fp == nullptr) return modules;

    char line[1024];
    std::set<std::string> seen;
    while (fgets(line, sizeof(line), fp) != nullptr) {
        const char* slash = strchr(line, '/');
        if (slash == nullptr) continue;
        std::string path(slash);
        while (!path.empty() && (path.back() == '\n' || path.back() == '\r')) {
            path.pop_back();
        }
        if (!path.empty() && seen.insert(path).second) modules.push_back(path);
    }
    fclose(fp);
    return modules;
}

std::string modulePathForAddress(uintptr_t addr) {
    Dl_info info;
    memset(&info, 0, sizeof(info));
    if (dladdr(reinterpret_cast<void*>(addr), &info) == 0 || info.dli_fname == nullptr) {
        return std::string();
    }
    return std::string(info.dli_fname);
}

uintptr_t libraryBase(const std::string& module) {
    std::lock_guard<std::mutex> lock(Mutex());
    return resolveBaseLocked(module);
}

bool isLibraryLoaded(const std::string& module) {
    FILE* fp = fopen("/proc/self/maps", "re");
    if (fp == nullptr) return false;

    char line[1024];
    bool found = false;
    while (fgets(line, sizeof(line), fp) != nullptr) {
        if (strstr(line, module.c_str()) != nullptr) {
            found = true;
            break;
        }
    }
    fclose(fp);
    return found;
}

// ---------------------------------------------------------------- symbols

uintptr_t symbolAddress(const std::string& module, const std::string& symbol) {
    void* handle = xdl_open(module.c_str(), XDL_DEFAULT);
    if (handle == nullptr) {
        const size_t slash = module.rfind('/');
        if (slash != std::string::npos) {
            handle = xdl_open(module.c_str() + slash + 1, XDL_DEFAULT);
        }
    }
    if (handle == nullptr) {
        LOGE("xdl_open failed for %s", module.c_str());
        return 0;
    }

    void* addr = xdl_sym(handle, symbol.c_str(), nullptr);
    if (addr == nullptr) {
        addr = xdl_dsym(handle, symbol.c_str(), nullptr);
    }
    xdl_close(handle);
    return reinterpret_cast<uintptr_t>(addr);
}

uintptr_t resolveAddress(const std::string& module, const std::string& offsetOrSymbol) {
    const uintptr_t offset = parseHexOffset(offsetOrSymbol);
    if (offset != 0) {
        const uintptr_t base = libraryBase(module);
        return base == 0 ? 0 : base + offset;
    }
    return symbolAddress(module, offsetOrSymbol);
}

// ---------------------------------------------------------------- hooking

bool hookFunction(uintptr_t target, void* replacement, void** original) {
    if (target == 0 || replacement == nullptr) {
        LOGE("hook: invalid target/replacement");
        return false;
    }
    const int rc = DobbyHook(reinterpret_cast<void*>(target),
                             reinterpret_cast<dobby_dummy_func_t>(replacement),
                             reinterpret_cast<dobby_dummy_func_t*>(original));
    if (rc != 0) {
        LOGE("hook failed at 0x%lx (rc=%d)", static_cast<unsigned long>(target), rc);
        return false;
    }
    LOGI("hooked 0x%lx", static_cast<unsigned long>(target));
    return true;
}

bool hookModule(const std::string& module, const std::string& offsetOrSymbol,
                void* replacement, void** original) {
    const uintptr_t target = resolveAddress(module, offsetOrSymbol);
    if (target == 0) {
        LOGE("hook: cannot resolve %s in %s", offsetOrSymbol.c_str(), module.c_str());
        return false;
    }
    return hookFunction(target, replacement, original);
}

bool unhookFunction(uintptr_t target) {
    if (target == 0) return false;
    const int rc = DobbyDestroy(reinterpret_cast<void*>(target));
    return rc == 0;
}

// ---------------------------------------------------------------- patching

bool patchBytes(uintptr_t target, const void* bytes, size_t size) {
    if (target == 0 || bytes == nullptr || size == 0) return false;

    std::lock_guard<std::mutex> lock(Mutex());

    // Re-patching the same address must replace the previous patch, not
    // re-apply its old bytes. Undo the old one first so the "original" that
    // MemoryPatch captures is the real code.
    const auto existing = Patches().find(target);
    if (existing != Patches().end()) {
        existing->second.Restore();
        Patches().erase(existing);
    }

    MemoryPatch patch = MemoryPatch::createWithBytes(target, bytes, size);
    if (!patch.isValid()) {
        LOGE("patch: createWithBytes failed at 0x%lx", static_cast<unsigned long>(target));
        return false;
    }
    if (!patch.Modify()) {
        LOGE("patch: Modify failed at 0x%lx", static_cast<unsigned long>(target));
        return false;
    }
    Patches()[target] = patch;
    return true;
}

bool restorePatch(uintptr_t target) {
    std::lock_guard<std::mutex> lock(Mutex());
    const auto it = Patches().find(target);
    if (it == Patches().end()) return false;
    const bool ok = it->second.Restore();
    if (ok) Patches().erase(it);
    return ok;
}

bool readBytes(uintptr_t target, void* out, size_t size) {
    if (target == 0 || out == nullptr || size == 0) return false;
    memcpy(out, reinterpret_cast<const void*>(target), size);
    return true;
}

}  // namespace bsu
