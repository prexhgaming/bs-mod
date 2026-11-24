// libbs_dumper.so — CPython runtime extractor for the sandboxed game process.
//
// The target game statically links CPython into libGame.so, so the interpreter
// only exists inside the game's process. This library is loaded there (see
// PySdkDumper / BlackBoxLoader) while the launcher's dumper toggle is on.
//
// There is deliberately NO JNI entry point at all. A `__attribute__((constructor))`
// runs as soon as the dynamic linker loads this library, and it immediately
// detaches a worker thread, so the game's main thread is never blocked and no
// value ever has to cross JNI. The worker reads /proc/self/maps to work out
//   * the game's package name,
//   * its native-library directory (where libGame.so lives),
//   * the BlackBox sandbox layout (virtual data dir),
// then writes the dump to
//   /storage/emulated/0/Android/data/<game-pkg>/sdk/
// which needs no storage permission at all (app-specific external storage).

#include <android/log.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <algorithm>
#include <cctype>
#include <cerrno>
#include <chrono>
#include <cstdarg>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <mutex>
#include <set>
#include <string>
#include <thread>
#include <vector>

#include "native_utils.hpp"

#define TAG "BsSdkDumper"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)

namespace {

// --------------------------------------------------------------- limits

// Buffer sizes/offsets: size_t, so they interoperate with the size_t values
// produced by fread()/std::string on both 32-bit and 64-bit ABIs.
constexpr size_t kChunk = 8u * 1024u * 1024u;             // scan chunk
constexpr size_t kOverlap = 64;                           // banner may straddle
constexpr uint64_t kMaxScanBytes = 512ull * 1024 * 1024;  // per library
constexpr uint64_t kMaxSingleLib = 160ull * 1024 * 1024;  // copied per file
constexpr uint64_t kMaxLibBytes = 1024ull * 1024 * 1024;  // libs total
constexpr uint64_t kMaxPyBytes = 512ull * 1024 * 1024;    // python files total
constexpr int kMaxPyFiles = 8192;
constexpr int kMaxWalkDepth = 10;

// --------------------------------------------------------------- logging

// Function-local statics on purpose: this library starts its worker from a
// `__attribute__((constructor))`, which may run before namespace-scope C++
// objects in this translation unit are constructed. A Meyers singleton is
// built on first use, so it is safe from any thread at any time.
std::mutex& LogMutex() {
    static std::mutex m;
    return m;
}

std::string& LogBuffer() {
    static std::string s;
    return s;
}

void Logf(const char* fmt, ...) {
    char line[1024];
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(line, sizeof(line), fmt, ap);
    va_end(ap);

    LOGI("%s", line);
    std::lock_guard<std::mutex> lock(LogMutex());
    time_t now = time(nullptr);
    char stamp[32];
    struct tm tmv;
    localtime_r(&now, &tmv);
    strftime(stamp, sizeof(stamp), "%H:%M:%S", &tmv);
    std::string& log = LogBuffer();
    log += stamp;
    log += "  ";
    log += line;
    log += '\n';
}

std::string TakeLog() {
    std::lock_guard<std::mutex> lock(LogMutex());
    return LogBuffer();
}

// --------------------------------------------------------------- fs helpers

std::string Join(const std::string& dir, const std::string& name) {
    if (dir.empty()) return name;
    if (dir.back() == '/') return dir + name;
    return dir + "/" + name;
}

std::string BaseName(const std::string& path) {
    const size_t slash = path.rfind('/');
    return slash == std::string::npos ? path : path.substr(slash + 1);
}

std::string DirName(const std::string& path) {
    const size_t slash = path.rfind('/');
    return slash == std::string::npos ? std::string() : path.substr(0, slash);
}

bool EndsWith(const std::string& s, const std::string& tail) {
    return s.size() >= tail.size() &&
           s.compare(s.size() - tail.size(), tail.size(), tail) == 0;
}

bool IsDir(const std::string& path) {
    struct stat st;
    return stat(path.c_str(), &st) == 0 && S_ISDIR(st.st_mode);
}

bool IsFile(const std::string& path) {
    struct stat st;
    return stat(path.c_str(), &st) == 0 && S_ISREG(st.st_mode);
}

uint64_t FileSize(const std::string& path) {
    struct stat st;
    return stat(path.c_str(), &st) == 0 ? static_cast<uint64_t>(st.st_size) : 0;
}

bool MakeDirs(const std::string& path) {
    if (path.empty()) return false;
    std::string cur;
    size_t i = 0;
    if (path[0] == '/') {
        cur = "/";
        i = 1;
    }
    while (i <= path.size()) {
        const size_t slash = path.find('/', i);
        const std::string part = path.substr(
                i, slash == std::string::npos ? std::string::npos : slash - i);
        if (!part.empty()) {
            if (!cur.empty() && cur.back() != '/') cur += '/';
            cur += part;
            if (mkdir(cur.c_str(), 0770) != 0 && errno != EEXIST) {
                struct stat st;
                if (stat(cur.c_str(), &st) != 0 || !S_ISDIR(st.st_mode)) return false;
            }
        }
        if (slash == std::string::npos) break;
        i = slash + 1;
    }
    return true;
}

// mkdir -p plus a real write, so the caller never starts copying into a
// directory the OS will refuse later.
bool IsWritableDir(const std::string& dir) {
    if (!MakeDirs(dir)) return false;
    const std::string probe = Join(dir, ".probe");
    FILE* f = fopen(probe.c_str(), "wb");
    if (f == nullptr) return false;
    fputc('1', f);
    fclose(f);
    unlink(probe.c_str());
    return true;
}

bool ReadWhole(const std::string& path, std::string* out) {
    FILE* f = fopen(path.c_str(), "rb");
    if (f == nullptr) return false;
    char buf[65536];
    size_t n;
    while ((n = fread(buf, 1, sizeof(buf), f)) > 0) out->append(buf, n);
    fclose(f);
    return true;
}

bool WriteText(const std::string& path, const std::string& text) {
    FILE* f = fopen(path.c_str(), "wb");
    if (f == nullptr) return false;
    const bool ok = fwrite(text.data(), 1, text.size(), f) == text.size();
    fclose(f);
    return ok;
}

bool CopyFile(const std::string& from, const std::string& to, uint64_t maxBytes) {
    const uint64_t size = FileSize(from);
    if (size == 0 || size > maxBytes) return false;

    FILE* in = fopen(from.c_str(), "rb");
    if (in == nullptr) return false;
    FILE* out = fopen(to.c_str(), "wb");
    if (out == nullptr) {
        fclose(in);
        return false;
    }

    std::vector<char> buf(1024 * 1024);
    bool ok = true;
    size_t n;
    while ((n = fread(buf.data(), 1, buf.size(), in)) > 0) {
        if (fwrite(buf.data(), 1, n, out) != n) {
            ok = false;
            break;
        }
    }
    fclose(in);
    if (fclose(out) != 0) ok = false;
    if (!ok) unlink(to.c_str());
    return ok;
}

std::string JsonEscape(const std::string& raw) {
    std::string out;
    out.reserve(raw.size() + 8);
    for (char c : raw) {
        switch (c) {
            case '"': out += "\\\""; break;
            case '\\': out += "\\\\"; break;
            case '\n': out += "\\n"; break;
            case '\r': out += "\\r"; break;
            case '\t': out += "\\t"; break;
            default:
                if (static_cast<unsigned char>(c) < 0x20) {
                    char esc[8];
                    snprintf(esc, sizeof(esc), "\\u%04x", c & 0xff);
                    out += esc;
                } else {
                    out += c;
                }
        }
    }
    return out;
}

std::vector<std::string> ListDir(const std::string& dir) {
    std::vector<std::string> names;
    DIR* d = opendir(dir.c_str());
    if (d == nullptr) return names;
    struct dirent* e;
    while ((e = readdir(d)) != nullptr) {
        if (strcmp(e->d_name, ".") == 0 || strcmp(e->d_name, "..") == 0) continue;
        names.emplace_back(e->d_name);
    }
    closedir(d);
    std::sort(names.begin(), names.end());
    return names;
}

// --------------------------------------------------------------- discovery

struct Discovery {
    std::string gamePkg;      // com.netease.newspike
    std::string hostPkg;      // top.niunaijun.blackboxa
    std::string sandboxRoot;  // <host data>/blackbox
    std::string appDir;       // <sandboxRoot>/data/app/<gamePkg>
    std::string libDir;       // native libs actually loaded by the game
    std::string sandboxLibDir;
    std::string dataDir;      // <sandboxRoot>/data/user/<uid>/<gamePkg>
    std::vector<std::string> modules;
    bool ok = false;
};

// Package names are [A-Za-z0-9._] with at least one dot; a split-install
// directory is "<pkg>-<base64hash>==", so the first '-' ends the package.
bool LooksLikePackage(const std::string& s) {
    if (s.size() < 4) return false;
    int dots = 0;
    for (char c : s) {
        const unsigned char u = static_cast<unsigned char>(c);
        if (c == '.') {
            ++dots;
        } else if (!isalnum(u) && c != '_') {
            return false;
        }
    }
    return dots >= 2;
}

std::vector<std::string> SplitPath(const std::string& path) {
    std::vector<std::string> parts;
    size_t i = 0;
    while (i < path.size()) {
        while (i < path.size() && path[i] == '/') ++i;
        const size_t start = i;
        while (i < path.size() && path[i] != '/') ++i;
        if (i > start) parts.push_back(path.substr(start, i - start));
    }
    return parts;
}

// Every mapped path, in order, from /proc/self/maps.
std::vector<std::string> MapsPaths(const std::string& maps) {
    std::vector<std::string> paths;
    size_t pos = 0;
    while (pos < maps.size()) {
        size_t nl = maps.find('\n', pos);
        if (nl == std::string::npos) nl = maps.size();
        const std::string line = maps.substr(pos, nl - pos);
        pos = nl + 1;

        // "addr perms offset dev inode path"
        const size_t slash = line.find('/');
        if (slash == std::string::npos) continue;
        std::string path = line.substr(slash);
        while (!path.empty() && (path.back() == '\r' || path.back() == ' ')) {
            path.pop_back();
        }
        if (!path.empty()) paths.push_back(path);
    }
    return paths;
}

// "<sandbox>/data/app/<pkg>-<hash>==/..." -> "<pkg>"
std::string PackageFromAppPath(const std::string& path) {
    const std::vector<std::string> parts = SplitPath(path);
    for (size_t i = 0; i + 1 < parts.size(); ++i) {
        if (parts[i] != "app") continue;
        for (size_t j = i + 1; j < parts.size(); ++j) {
            const std::string& seg = parts[j];
            if (!EndsWith(seg, "==")) continue;
            const size_t dash = seg.find('-');
            if (dash == std::string::npos) continue;
            const std::string pkg = seg.substr(0, dash);
            if (LooksLikePackage(pkg)) return pkg;
        }
    }
    return std::string();
}

Discovery Discover(const std::vector<std::string>& paths) {
    Discovery d;

    // 1. Sandbox root: the engine keeps everything under <host data>/blackbox.
    for (const std::string& path : paths) {
        const size_t at = path.find("/blackbox/");
        if (at == std::string::npos) continue;
        d.sandboxRoot = path.substr(0, at + strlen("/blackbox"));
        const std::vector<std::string> parts = SplitPath(path.substr(0, at));
        if (!parts.empty()) d.hostPkg = parts.back();
        break;
    }

    // 2. Candidate packages from /data/app/<pkg>-<hash>==/ entries.
    std::vector<std::string> candidates;
    for (const std::string& path : paths) {
        if (path.find("/data/app/") == std::string::npos) continue;
        const std::string pkg = PackageFromAppPath(path);
        if (pkg.empty()) continue;
        if (!candidates.empty() && candidates.back() == pkg) continue;
        candidates.push_back(pkg);
    }

    // 3. The game is the candidate that is not the host app. Prefer one whose
    //    sandbox directory actually exists on disk.
    for (const std::string& pkg : candidates) {
        if (!d.hostPkg.empty() && pkg == d.hostPkg) continue;
        if (!d.sandboxRoot.empty() &&
            IsDir(Join(d.sandboxRoot, "data/app/" + pkg))) {
            d.gamePkg = pkg;
            break;
        }
    }
    if (d.gamePkg.empty()) {
        for (const std::string& pkg : candidates) {
            if (!d.hostPkg.empty() && pkg == d.hostPkg) continue;
            d.gamePkg = pkg;
            break;
        }
    }
    if (d.gamePkg.empty()) return d;

    // 4. Native-library directories: the sandbox copy (what the engine stages)
    //    and the real split-install dir the loader mapped libGame.so from.
    if (!d.sandboxRoot.empty()) {
        d.appDir = Join(d.sandboxRoot, "data/app/" + d.gamePkg);
        const std::string staged = Join(d.appDir, "lib");
        if (IsDir(staged)) d.sandboxLibDir = staged;
    }
    for (const std::string& path : paths) {
        if (path.find("/data/app/") == std::string::npos) continue;
        if (PackageFromAppPath(path) != d.gamePkg) continue;
        const size_t libAt = path.find("/lib/");
        if (libAt == std::string::npos) continue;
        d.libDir = path.substr(0, libAt + strlen("/lib"));
        break;
    }
    if (d.libDir.empty()) d.libDir = d.sandboxLibDir;

    // 5. Virtual data dir: <sandbox>/data/user/<uid>/<gamePkg>.
    if (!d.sandboxRoot.empty()) {
        const std::string users = Join(d.sandboxRoot, "data/user");
        for (const std::string& uid : ListDir(users)) {
            const std::string candidate = Join(users, uid + "/" + d.gamePkg);
            if (IsDir(candidate)) {
                d.dataDir = candidate;
                break;
            }
        }
    }

    d.ok = !d.libDir.empty() && IsDir(d.libDir);
    return d;
}

// --------------------------------------------------------------- python scan

// Py_GetVersion() reports "3.11.2 (v3.11.2:...)"; the trailing " (" is what
// makes the match specific enough not to fire on random version strings.
std::string FindPythonVersion(const std::string& path, uint64_t* scanned) {
    FILE* f = fopen(path.c_str(), "rb");
    if (f == nullptr) return std::string();

    std::vector<char> buf(kChunk + kOverlap);
    uint64_t total = 0;
    size_t carry = 0;
    std::string found;

    while (total < kMaxScanBytes) {
        const size_t n = fread(buf.data() + carry, 1, kChunk, f);
        if (n == 0) break;
        total += n;
        const size_t len = carry + n;

        for (size_t i = 0; i + 6 < len; ++i) {
            if (buf[i] != '3' || buf[i + 1] != '.') continue;
            if (i > 0 && isdigit(static_cast<unsigned char>(buf[i - 1]))) continue;

            size_t j = i + 2;
            int major = 0;
            while (j < len && isdigit(static_cast<unsigned char>(buf[j]))) {
                ++j;
                ++major;
            }
            if (major == 0 || j >= len || buf[j] != '.') continue;
            ++j;
            int minor = 0;
            while (j < len && isdigit(static_cast<unsigned char>(buf[j]))) {
                ++j;
                ++minor;
            }
            if (minor == 0) continue;

            size_t k = j;
            if (k < len && buf[k] == '.') {
                ++k;
                int patch = 0;
                while (k < len && isdigit(static_cast<unsigned char>(buf[k]))) {
                    ++k;
                    ++patch;
                }
                if (patch == 0) continue;
            }
            if (k + 1 < len && buf[k] == ' ' && buf[k + 1] == '(') {
                found.assign(buf.data() + i, k - i);
                break;
            }
        }
        if (!found.empty()) break;

        carry = std::min(kOverlap, len);
        memmove(buf.data(), buf.data() + len - carry, carry);
    }
    fclose(f);
    if (scanned != nullptr) *scanned = total;
    return found;
}

struct PythonHit {
    std::string lib;      // absolute path
    std::string version;  // 3.11.2
    uint64_t size = 0;
};

// --------------------------------------------------------------- dump

struct Stats {
    int libs = 0;
    uint64_t libBytes = 0;
    int pyFiles = 0;
    uint64_t pyBytes = 0;
    int failed = 0;
};

std::string OutputDirFor(const Discovery& d) {
    // Primary: the game's own app-specific external dir. Needs no permission.
    const std::string primary =
            "/storage/emulated/0/Android/data/" + d.gamePkg + "/sdk";
    if (IsWritableDir(primary)) return primary;

    // Fallback: our own app-specific external dir (always writable).
    if (!d.hostPkg.empty()) {
        const std::string alt =
                "/storage/emulated/0/Android/data/" + d.hostPkg + "/files/sdk/" + d.gamePkg;
        if (IsWritableDir(alt)) return alt;
        const std::string internal =
                "/data/user/0/" + d.hostPkg + "/files/sdk/" + d.gamePkg;
        if (IsWritableDir(internal)) return internal;
    }
    return std::string();
}

void CopyLibs(const Discovery& d, const std::string& outDir, Stats* st) {
    const std::string dest = Join(outDir, "libs");
    MakeDirs(dest);

    std::set<std::string> seen;
    std::vector<std::string> dirs;
    if (!d.libDir.empty()) dirs.push_back(d.libDir);
    if (!d.sandboxLibDir.empty() && d.sandboxLibDir != d.libDir) {
        dirs.push_back(d.sandboxLibDir);
    }

    for (const std::string& dir : dirs) {
        for (const std::string& name : ListDir(dir)) {
            if (!EndsWith(name, ".so")) continue;
            if (!seen.insert(name).second) continue;

            const std::string src = Join(dir, name);
            if (!IsFile(src)) continue;

            const uint64_t size = FileSize(src);
            if (size > kMaxSingleLib) {
                Logf("skip %s: %llu bytes over per-file cap", name.c_str(),
                     static_cast<unsigned long long>(size));
                continue;
            }
            if (st->libBytes + size > kMaxLibBytes) {
                Logf("stop libs: total cap reached at %s", name.c_str());
                return;
            }
            if (CopyFile(src, Join(dest, name), kMaxSingleLib)) {
                st->libs += 1;
                st->libBytes += size;
                Logf("lib  %-40s %llu bytes", name.c_str(),
                     static_cast<unsigned long long>(size));
            } else {
                st->failed += 1;
                Logf("lib  %-40s FAILED", name.c_str());
            }
        }
    }
}

bool IsPythonFile(const std::string& name) {
    return EndsWith(name, ".py") || EndsWith(name, ".pyc") || EndsWith(name, ".pyi") ||
           EndsWith(name, ".pth") || EndsWith(name, ".pyd") || EndsWith(name, ".so.py");
}

void WalkPython(const std::string& root, const std::string& outDir, int depth,
                Stats* st) {
    if (depth > kMaxWalkDepth) return;
    for (const std::string& name : ListDir(root)) {
        if (st->pyFiles >= kMaxPyFiles || st->pyBytes >= kMaxPyBytes) return;
        const std::string path = Join(root, name);
        if (IsDir(path)) {
            WalkPython(path, outDir, depth + 1, st);
            continue;
        }
        if (!IsPythonFile(name)) continue;

        const uint64_t size = FileSize(path);
        if (st->pyBytes + size > kMaxPyBytes) return;

        std::string rel = path;
        if (rel.compare(0, root.size(), root) == 0) rel = rel.substr(root.size());
        while (!rel.empty() && rel.front() == '/') rel.erase(rel.begin());

        const std::string dest = Join(Join(outDir, "python"), rel);
        MakeDirs(DirName(dest));
        if (CopyFile(path, dest, kMaxPyBytes)) {
            st->pyFiles += 1;
            st->pyBytes += size;
        } else {
            st->failed += 1;
        }
    }
}

std::string BuildManifest(const Discovery& d, const std::string& outDir,
                          const PythonHit& py, const Stats& st,
                          const std::string& mapsPath) {
    std::string j = "{\n";
    j += "  \"generated\": " + std::to_string(static_cast<long long>(time(nullptr))) + ",\n";
    j += "  \"package\": \"" + JsonEscape(d.gamePkg) + "\",\n";
    j += "  \"hostPackage\": \"" + JsonEscape(d.hostPkg) + "\",\n";
    j += "  \"outputDir\": \"" + JsonEscape(outDir) + "\",\n";
    j += "  \"sandboxRoot\": \"" + JsonEscape(d.sandboxRoot) + "\",\n";
    j += "  \"appDir\": \"" + JsonEscape(d.appDir) + "\",\n";
    j += "  \"libDir\": \"" + JsonEscape(d.libDir) + "\",\n";
    j += "  \"dataDir\": \"" + JsonEscape(d.dataDir) + "\",\n";
    j += "  \"python\": {\n";
    j += "    \"version\": \"" + JsonEscape(py.version) + "\",\n";
    j += "    \"library\": \"" + JsonEscape(py.lib) + "\",\n";
    j += "    \"libraryBytes\": " + std::to_string(static_cast<unsigned long long>(py.size)) + "\n";
    j += "  },\n";
    j += "  \"libs\": { \"count\": " + std::to_string(st.libs) +
         ", \"bytes\": " + std::to_string(static_cast<unsigned long long>(st.libBytes)) + " },\n";
    j += "  \"pythonFiles\": { \"count\": " + std::to_string(st.pyFiles) +
         ", \"bytes\": " + std::to_string(static_cast<unsigned long long>(st.pyBytes)) + " },\n";
    j += "  \"maps\": \"" + JsonEscape(mapsPath) + "\",\n";
    j += "  \"libGameBase\": \"" + std::to_string(static_cast<unsigned long long>(bsu::libraryBase("libGame.so"))) + "\",\n";
    j += "  \"modules\": [";
    // native_utils: every mapped path, deduplicated, in maps order.
    const std::vector<std::string> modules = bsu::loadedModules();
    const size_t limit = std::min<size_t>(modules.size(), 200);
    for (size_t i = 0; i < limit; ++i) {
        if (i != 0) j += ", ";
        j += "\"" + JsonEscape(modules[i]) + "\"";
    }
    j += "]\n}\n";
    return j;
}

// --------------------------------------------------------------- worker

void RunDump() {
    Logf("worker started (pid %d)", getpid());

    // The engine maps the game's APKs and libs a little *after* this library is
    // loaded (libGame.so is ~84 MB, so it can take a while on a cold start).
    // Poll until the layout is visible, like the usual "wait for the target lib"
    // loop in mod menus, but with a hard deadline so we never spin forever.
    Discovery d;
    std::string mapsOut;
    const auto deadline = std::chrono::steady_clock::now() + std::chrono::seconds(90);
    int attempt = 0;
    for (;;) {
        // native_utils gives us the same answer through the hardened xDL path:
        // is the game's CPython library mapped yet?
        if (bsu::isLibraryLoaded("libGame.so")) {
            std::string maps;
            if (ReadWhole("/proc/self/maps", &maps)) {
                d = Discover(MapsPaths(maps));
            }
            if (d.ok) {
                Logf("discover: ready after %d attempt(s)", attempt + 1);
                mapsOut.swap(maps);
                break;
            }
        }
        if (std::chrono::steady_clock::now() >= deadline) {
            Logf("discover: gave up after %d attempts", attempt + 1);
            break;
        }
        if (attempt < 3 || attempt % 20 == 0) {
            Logf("discover attempt %d: game libs not mapped yet, waiting", attempt + 1);
        }
        ++attempt;
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
    }

    if (!d.ok) {
        Logf("FATAL no game native-lib dir found (gamePkg=%s hostPkg=%s)",
             d.gamePkg.c_str(), d.hostPkg.c_str());
        return;
    }

    // Resolve the game's base address and a handful of symbols through xDL
    // instead of parsing /proc/self/maps by hand. This doubles as a sanity
    // check that the library is really loaded the way we expect.
    const uintptr_t gameBase = bsu::libraryBase("libGame.so");
    Logf("game    : %s", d.gamePkg.c_str());
    Logf("host    : %s", d.hostPkg.c_str());
    Logf("sandbox : %s", d.sandboxRoot.c_str());
    Logf("libDir  : %s", d.libDir.c_str());
    Logf("dataDir : %s", d.dataDir.empty() ? "(none)" : d.dataDir.c_str());
    if (gameBase != 0) {
        Logf("libGame.so base: 0x%lx", static_cast<unsigned long>(gameBase));
    } else {
        Logf("libGame.so base: unresolved (xdl_open failed)");
    }

    const std::string outDir = OutputDirFor(d);
    if (outDir.empty()) {
        Logf("FATAL no writable output dir for %s", d.gamePkg.c_str());
        return;
    }
    Logf("output  : %s", outDir.c_str());

    WriteText(Join(outDir, "maps.txt"), mapsOut);

    // Locate CPython and keep the module list for the manifest.
    PythonHit py;
    std::vector<std::string> dirs;
    if (!d.libDir.empty()) dirs.push_back(d.libDir);
    if (!d.sandboxLibDir.empty() && d.sandboxLibDir != d.libDir) {
        dirs.push_back(d.sandboxLibDir);
    }

    std::set<std::string> scanned;
    for (const std::string& dir : dirs) {
        for (const std::string& name : ListDir(dir)) {
            if (!EndsWith(name, ".so")) continue;
            if (!scanned.insert(name).second) continue;
            const std::string path = Join(dir, name);
            if (!IsFile(path)) continue;
            d.modules.push_back(path);

            uint64_t bytes = 0;
            const std::string version = FindPythonVersion(path, &bytes);
            Logf("scan %-40s %8llu KB  %s", name.c_str(),
                 static_cast<unsigned long long>(bytes / 1024),
                 version.empty() ? "-" : version.c_str());
            if (!version.empty() && py.version.empty()) {
                py.lib = path;
                py.version = version;
                py.size = FileSize(path);
            }
        }
    }
    if (!py.version.empty()) {
        Logf("CPython %s in %s", py.version.c_str(), BaseName(py.lib).c_str());
    } else {
        Logf("no CPython banner found in %zu libraries", d.modules.size());
    }

    Stats st;
    CopyLibs(d, outDir, &st);

    if (!d.dataDir.empty()) {
        MakeDirs(Join(outDir, "python"));
        WalkPython(d.dataDir, outDir, 0, &st);
    }

    const std::string mapsPath = Join(outDir, "maps.txt");
    WriteText(Join(outDir, "env.json"),
              BuildManifest(d, outDir, py, st, mapsPath));

    Logf("done: %d libs (%llu bytes), %d python files (%llu bytes), %d failed",
         st.libs, static_cast<unsigned long long>(st.libBytes), st.pyFiles,
         static_cast<unsigned long long>(st.pyBytes), st.failed);

    WriteText(Join(outDir, "dump.log"), TakeLog());
}

}  // namespace

// Started by the dynamic linker as soon as this library is loaded, which is
// the whole point: no JNI_OnLoad, no Java call, no argument passing. Java only
// decides *whether* to dlopen us (see PySdkDumper.loadIfArmed).
//
// The body only spawns a thread and returns immediately, so the game's main
// thread is never blocked. Never do work directly in a constructor.
__attribute__((constructor)) void lib_main() {
    LOGI("constructor: spawning dump worker");
    std::thread(RunDump).detach();
}
