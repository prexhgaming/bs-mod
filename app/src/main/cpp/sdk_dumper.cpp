// libbs_dumper.so — in-process Python SDK dumper for the sandboxed game.
//
// Loaded by the host app *inside the game's own process* (the BlackBox engine
// runs the guest Application in a host-package process). Because we are already
// in the target process, Java does not have to hand us the game's directories:
// we discover the sandbox layout ourselves from /proc/self/maps.
//
// The one JNI entry point exists purely so Java can pass the single path only it
// can resolve — the public Downloads directory. JNI_OnLoad spawns a worker
// thread which waits for that path and then performs the dump.

#include <jni.h>
#include <android/log.h>
#include <dirent.h>
#include <dlfcn.h>
#include <pthread.h>
#include <sys/prctl.h>
#include <sys/stat.h>
#include <unistd.h>

#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <string>
#include <vector>

#define LOG_TAG "BsSdkDumper"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

namespace {

// ---------------------------------------------------------------- tuning

// How long the worker waits for Java to hand it the Downloads path.
constexpr int kConfigTimeoutSec = 45;

// Scanning: 84 MB libGame.so must fit, so no small cap; read in chunks.
constexpr size_t kScanChunk = 4 * 1024 * 1024;
constexpr int kMaxScanBytes = 512 * 1024 * 1024;

// Copy limits, so a pathological tree cannot fill the disk.
constexpr int kMaxPyFiles = 8192;
constexpr long long kMaxPyBytes = 512LL * 1024 * 1024;
constexpr long long kMaxLibBytes = 512LL * 1024 * 1024;
constexpr long long kMaxSingleLibBytes = 160LL * 1024 * 1024;
constexpr int kMaxWalkDepth = 12;

// ---------------------------------------------------------------- state

pthread_mutex_t gLock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t gCond = PTHREAD_COND_INITIALIZER;
std::string gOutDir;      // set by nativeStart()
bool gArmed = false;      // guarded by gLock
bool gThreadStarted = false;

JavaVM* gVm = nullptr;    // cached; the worker makes no Java calls

// ---------------------------------------------------------------- strings

std::string JStr(JNIEnv* env, jstring s) {
    if (s == nullptr) return {};
    const char* c = env->GetStringUTFChars(s, nullptr);
    if (c == nullptr) return {};
    std::string out(c);
    env->ReleaseStringUTFChars(s, c);
    return out;
}

std::string Join(const std::string& a, const std::string& b) {
    if (a.empty()) return b;
    if (b.empty()) return a;
    if (a.back() == '/') return a + b;
    return a + "/" + b;
}

std::string DirName(const std::string& path) {
    const size_t slash = path.rfind('/');
    if (slash == std::string::npos || slash == 0) return "/";
    return path.substr(0, slash);
}

std::string BaseName(const std::string& path) {
    const size_t slash = path.rfind('/');
    if (slash == std::string::npos) return path;
    return path.substr(slash + 1);
}

bool EndsWith(const std::string& s, const std::string& suffix) {
    return s.size() >= suffix.size() &&
           s.compare(s.size() - suffix.size(), suffix.size(), suffix) == 0;
}

bool StartsWith(const std::string& s, const std::string& prefix) {
    return s.size() >= prefix.size() && s.compare(0, prefix.size(), prefix) == 0;
}

// ---------------------------------------------------------------- filesystem

bool Exists(const std::string& path) {
    struct stat st {};
    return stat(path.c_str(), &st) == 0;
}

bool IsDir(const std::string& path) {
    struct stat st {};
    return stat(path.c_str(), &st) == 0 && S_ISDIR(st.st_mode);
}

long long FileSize(const std::string& path) {
    struct stat st {};
    if (stat(path.c_str(), &st) != 0) return -1;
    return static_cast<long long>(st.st_size);
}

bool MakeDirs(const std::string& path) {
    if (path.empty()) return false;
    std::string acc;
    acc.reserve(path.size());
    size_t i = 0;
    if (path[0] == '/') {
        acc.push_back('/');
        i = 1;
    }
    while (i <= path.size()) {
        const size_t slash = path.find('/', i);
        const std::string part =
                path.substr(i, slash == std::string::npos ? std::string::npos : slash - i);
        if (!part.empty()) {
            acc = acc.empty() || acc.back() == '/' ? acc + part : acc + "/" + part;
            if (mkdir(acc.c_str(), 0775) != 0 && !IsDir(acc)) return false;
        }
        if (slash == std::string::npos) break;
        i = slash + 1;
    }
    return IsDir(path);
}

bool ReadWhole(const std::string& path, std::string* out) {
    FILE* f = fopen(path.c_str(), "rb");
    if (f == nullptr) return false;
    char buf[64 * 1024];
    size_t n;
    while ((n = fread(buf, 1, sizeof(buf), f)) > 0) out->append(buf, n);
    fclose(f);
    return true;
}

bool WriteText(const std::string& path, const std::string& text) {
    FILE* f = fopen(path.c_str(), "wb");
    if (f == nullptr) return false;
    const size_t wrote = fwrite(text.data(), 1, text.size(), f);
    fclose(f);
    return wrote == text.size();
}

bool CopyFile(const std::string& src, const std::string& dst) {
    FILE* in = fopen(src.c_str(), "rb");
    if (in == nullptr) return false;
    FILE* out = fopen(dst.c_str(), "wb");
    if (out == nullptr) {
        fclose(in);
        return false;
    }
    std::vector<char> buf(256 * 1024);
    bool ok = true;
    size_t n;
    while ((n = fread(buf.data(), 1, buf.size(), in)) > 0) {
        if (fwrite(buf.data(), 1, n, out) != n) {
            ok = false;
            break;
        }
    }
    fclose(in);
    fclose(out);
    if (!ok) { remove(dst.c_str()); return false; }
    return true;
}

// Recursive walk with a per-entry callback. `rel` is the path below `root`.
void Walk(const std::string& root, const std::string& rel, int depth,
          const std::function<void(const std::string& abs,
                                   const std::string& relPath)>& cb) {
    if (depth > kMaxWalkDepth) return;
    const std::string dir = rel.empty() ? root : Join(root, rel);
    DIR* d = opendir(dir.c_str());
    if (d == nullptr) return;
    struct dirent* e;
    while ((e = readdir(d)) != nullptr) {
        const std::string name = e->d_name;
        if (name == "." || name == "..") continue;
        const std::string childRel = rel.empty() ? name : rel + "/" + name;
        const std::string abs = Join(root, childRel);
        if (IsDir(abs)) {
            Walk(root, childRel, depth + 1, cb);
        } else {
            cb(abs, childRel);
        }
    }
    closedir(d);
}

// ---------------------------------------------------------------- discovery

struct Discovered {
    std::string process;   // /proc/self/cmdline
    std::string root;      // .../<host data>/blackbox
    std::string pkg;       // com.netease.newspike
    std::string appDir;    // <root>/data/app/<pkg>
    std::string libDir;    // <root>/data/app/<pkg>/lib
    std::string dataDir;   // <root>/data/user/<uid>/<pkg>
    std::string apkPath;
    std::vector<std::string> modules;  // every path seen in maps
};

std::string ReadCmdline() {
    std::string s;
    ReadWhole("/proc/self/cmdline", &s);
    const size_t nul = s.find('\0');
    if (nul != std::string::npos) s.resize(nul);
    return s;
}

std::vector<std::string> ParseMapsPaths(const std::string& maps) {
    std::vector<std::string> paths;
    size_t pos = 0;
    while (pos < maps.size()) {
        size_t nl = maps.find('\n', pos);
        if (nl == std::string::npos) nl = maps.size();
        const std::string line = maps.substr(pos, nl - pos);
        pos = nl + 1;
        // "<addr> perms offset dev inode   /path/to/file"
        size_t slash = line.find('/');
        if (slash == std::string::npos) continue;
        // Guard against a '/' inside the address/perms fields.
        const size_t fields = line.find("  ");
        if (fields != std::string::npos && slash < fields) {
            slash = line.find('/', fields);
            if (slash == std::string::npos) continue;
        }
        std::string path = line.substr(slash);
        while (!path.empty() && (path.back() == ' ' || path.back() == '\r')) path.pop_back();
        if (!path.empty()) paths.push_back(path);
    }
    return paths;
}

Discovered Discover() {
    Discovered d;
    d.process = ReadCmdline();

    std::string maps;
    if (!ReadWhole("/proc/self/maps", &maps)) {
        LOGW("discover: cannot read /proc/self/maps");
        return d;
    }
    d.modules = ParseMapsPaths(maps);

    // The engine keeps the guest copy of the APK at
    // <root>/data/app/<pkg>/base.apk and maps it, which is what pins down both
    // the sandbox root and the package name.
    for (const std::string& p : d.modules) {
        const size_t at = p.find("/data/app/");
        if (at == std::string::npos) continue;
        if (!EndsWith(p, ".apk")) continue;
        d.apkPath = p;
        d.appDir = DirName(p);
        d.pkg = BaseName(d.appDir);
        d.root = d.appDir.substr(0, at);  // strip "/data/app/<pkg>"
        break;
    }
    if (d.appDir.empty()) {
        LOGW("discover: no sandbox APK mapping found");
        return d;
    }
    d.libDir = Join(d.appDir, "lib");

    // Data dir: <root>/data/user/<uid>/<pkg>. The uid is not getuid() (the
    // sandbox is not a real Android user), so read it from the layout.
    const std::string userRoot = Join(d.root, "data/user");
    DIR* ud = opendir(userRoot.c_str());
    if (ud != nullptr) {
        struct dirent* e;
        std::string fallback;
        while ((e = readdir(ud)) != nullptr) {
            const std::string uid = e->d_name;
            if (uid == "." || uid == "..") continue;
            const std::string candidate = Join(Join(userRoot, uid), d.pkg);
            if (!IsDir(candidate)) continue;
            if (uid == "0") { d.dataDir = candidate; break; }
            if (fallback.empty()) fallback = candidate;
        }
        closedir(ud);
        if (d.dataDir.empty()) d.dataDir = fallback;
    }

    LOGI("discover: pkg=%s process=%s lib=%s data=%s",
         d.pkg.c_str(), d.process.c_str(), d.libDir.c_str(), d.dataDir.c_str());
    return d;
}

// ---------------------------------------------------------------- python

struct PyFinding {
    std::string version;   // "3.11.2"
    bool sawCpython = false;
    std::string source;    // file the version came from
};

// Looks for the Py_GetVersion banner ("3.11.2 (main, ...)") and the literal
// "CPython" in a binary, scanned in chunks with a small overlap.
bool ScanBinaryForPython(const std::string& path, PyFinding* out) {
    FILE* f = fopen(path.c_str(), "rb");
    if (f == nullptr) return false;

    const std::string cpython = "CPython";
    const size_t overlap = 64;
    std::vector<char> buf(kScanChunk + overlap);
    size_t carry = 0;
    long long scanned = 0;
    bool found = false;

    while (scanned < kMaxScanBytes) {
        const size_t n = fread(buf.data() + carry, 1, kScanChunk, f);
        if (n == 0) break;
        scanned += static_cast<long long>(n);
        const size_t total = carry + n;
        const std::string chunk(buf.data(), total);

        if (!out->sawCpython && chunk.find(cpython) != std::string::npos) {
            out->sawCpython = true;
            found = true;
        }

        // <digits>.<digits>.<digits> followed by " (" -> Py_GetVersion banner.
        for (size_t i = 0; i + 8 < chunk.size(); ++i) {
            if (!isdigit(static_cast<unsigned char>(chunk[i]))) continue;
            size_t j = i;
            int dots = 0;
            while (j < chunk.size() &&
                   (isdigit(static_cast<unsigned char>(chunk[j])) || chunk[j] == '.')) {
                if (chunk[j] == '.') dots++;
                j++;
            }
            if (dots != 2) continue;
            if (j >= chunk.size() || chunk[j] != ' ') continue;
            if (j + 1 >= chunk.size() || chunk[j + 1] != '(') continue;
            const std::string version = chunk.substr(i, j - i);
            if (version.size() < 5 || version.size() > 12) continue;
            if (out->version.empty()) {
                out->version = version;
                out->source = path;
            }
            found = true;
            break;
        }
        if (!out->version.empty() && out->sawCpython) break;

        carry = total < overlap ? total : overlap;
        memmove(buf.data(), chunk.data() + (total - carry), carry);
    }
    fclose(f);

    // A "3.11.2 (main, ...)" hit is only credible with CPython nearby.
    if (!out->version.empty() && !out->sawCpython) out->version.clear();
    return found;
}

bool LooksLikePythonFile(const std::string& name) {
    static const char* kSuffixes[] = {".py", ".pyc", ".pyo", ".pyd", ".pyz", ".mpk",
                                      ".zip", ".whl", ".egg"};
    for (const char* s : kSuffixes) {
        if (EndsWith(name, s)) return true;
    }
    return false;
}

// ---------------------------------------------------------------- json

std::string JsonEscape(const std::string& raw) {
    std::string out;
    out.reserve(raw.size() + 8);
    for (const char c : raw) {
        switch (c) {
            case '"': out += "\\\""; break;
            case '\\': out += "\\\\"; break;
            case '\n': out += "\\n"; break;
            case '\r': out += "\\r"; break;
            case '\t': out += "\\t"; break;
            default:
                if (static_cast<unsigned char>(c) < 0x20) {
                    char buf[8];
                    snprintf(buf, sizeof(buf), "\\u%04x", c);
                    out += buf;
                } else {
                    out.push_back(c);
                }
        }
    }
    return out;
}

std::string JsonStr(const std::string& s) { return "\"" + JsonEscape(s) + "\""; }

// ---------------------------------------------------------------- dump

struct DumpStats {
    int pyFiles = 0;
    long long pyBytes = 0;
    int libs = 0;
    long long libBytes = 0;
};

void CollectPythonFiles(const Discovered& d, const std::string& outDir, DumpStats* st) {
    if (d.dataDir.empty() || !IsDir(d.dataDir)) return;
    const std::string pyOut = Join(outDir, "python");
    Walk(d.dataDir, "", 0, [&](const std::string& abs, const std::string& rel) {
        if (st->pyFiles >= kMaxPyFiles || st->pyBytes >= kMaxPyBytes) return;
        if (!LooksLikePythonFile(BaseName(abs))) return;
        const long long size = FileSize(abs);
        if (size <= 0) return;
        const std::string dst = Join(pyOut, rel);
        if (!MakeDirs(DirName(dst))) return;
        if (!CopyFile(abs, dst)) return;
        st->pyFiles++;
        st->pyBytes += size;
    });
}

void CollectPythonLibs(const Discovered& d, const std::string& outDir, PyFinding* py,
                       DumpStats* st) {
    if (d.libDir.empty() || !IsDir(d.libDir)) return;
    const std::string libOut = Join(outDir, "libs");

    // Scan every .so below the sandbox lib dir; the interpreter is statically
    // linked into one of them (libGame.so for this title).
    std::vector<std::string> candidates;
    Walk(d.libDir, "", 0, [&](const std::string& abs, const std::string&) {
        if (EndsWith(BaseName(abs), ".so")) candidates.push_back(abs);
    });
    LOGI("lib scan: %zu candidates", candidates.size());

    for (const std::string& so : candidates) {
        PyFinding local;
        if (!ScanBinaryForPython(so, &local)) continue;
        if (py->version.empty() && !local.version.empty()) {
            py->version = local.version;
            py->source = local.source;
        }
        py->sawCpython = py->sawCpython || local.sawCpython;
        if (py->version.empty() && !local.sawCpython) continue;

        const long long size = FileSize(so);
        if (size <= 0 || size > kMaxSingleLibBytes) {
            LOGW("lib too large to copy (%lld): %s",
                 static_cast<long long>(size), BaseName(so).c_str());
            continue;
        }
        if (st->libBytes + size > kMaxLibBytes) continue;
        if (!MakeDirs(libOut)) continue;
        const std::string dst = Join(libOut, BaseName(so));
        if (!CopyFile(so, dst)) continue;
        st->libs++;
        st->libBytes += size;
    }
}

std::string BuildManifest(const Discovered& d, const PyFinding& py,
                          const DumpStats& st, const std::string& outDir) {
    std::string j = "{\n";
    j += "  \"generated_by\": \"libbs_dumper\",\n";
    j += "  \"pid\": " + std::to_string(getpid()) + ",\n";
    j += "  \"process\": " + JsonStr(d.process) + ",\n";
    j += "  \"package\": " + JsonStr(d.pkg) + ",\n";
    j += "  \"sandbox_root\": " + JsonStr(d.root) + ",\n";
    j += "  \"apk\": " + JsonStr(d.apkPath) + ",\n";
    j += "  \"app_dir\": " + JsonStr(d.appDir) + ",\n";
    j += "  \"native_lib_dir\": " + JsonStr(d.libDir) + ",\n";
    j += "  \"data_dir\": " + JsonStr(d.dataDir) + ",\n";
    j += "  \"output_dir\": " + JsonStr(outDir) + ",\n";
    j += "  \"python\": {\n";
    j += "    \"version\": " + JsonStr(py.version) + ",\n";
    j += "    \"version_source\": " + JsonStr(py.source) + ",\n";
    j += "    \"cpython_string_found\": " + std::string(py.sawCpython ? "true" : "false") +
         "\n";
    j += "  },\n";
    j += "  \"copied\": {\n";
    j += "    \"python_files\": " + std::to_string(st.pyFiles) + ",\n";
    j += "    \"python_bytes\": " + std::to_string(st.pyBytes) + ",\n";
    j += "    \"libs\": " + std::to_string(st.libs) + ",\n";
    j += "    \"lib_bytes\": " + std::to_string(st.libBytes) + "\n";
    j += "  },\n";
    j += "  \"loaded_modules\": [\n";
    for (size_t i = 0; i < d.modules.size(); ++i) {
        j += "    " + JsonStr(d.modules[i]);
        if (i + 1 < d.modules.size()) j += ",";
        j += "\n";
    }
    j += "  ]\n}\n";
    return j;
}

void RunDump(const std::string& outDir) {
    LOGI("dump start: out=%s", outDir.c_str());
    if (!MakeDirs(outDir)) {
        LOGE("dump: cannot create %s", outDir.c_str());
        return;
    }

    Discovered d = Discover();
    if (d.appDir.empty()) {
        // Nothing to point at: leave a breadcrumb so the user can see why.
        WriteText(Join(outDir, "env.json"),
                  "{\n  \"error\": \"sandbox layout not found in /proc/self/maps\"\n}\n");
        LOGE("dump: sandbox layout not found");
        return;
    }

    // Raw maps snapshot — cheap and the most useful forensic artifact.
    std::string maps;
    if (ReadWhole("/proc/self/maps", &maps)) {
        WriteText(Join(outDir, "maps.txt"), maps);
    }

    PyFinding py;
    DumpStats st;
    CollectPythonLibs(d, outDir, &py, &st);
    CollectPythonFiles(d, outDir, &st);

    WriteText(Join(outDir, "env.json"), BuildManifest(d, py, st, outDir));
    LOGI("dump done: py=%d (%lld B) libs=%d (%lld B) python=%s",
         st.pyFiles, static_cast<long long>(st.pyBytes), st.libs,
         static_cast<long long>(st.libBytes),
         py.version.empty() ? "<unknown>" : py.version.c_str());
}

// ---------------------------------------------------------------- worker

void* WorkerMain(void*) {
    prctl(PR_SET_NAME, "bs-sdk-dumper", 0, 0, 0);

    std::string outDir;
    {
        pthread_mutex_lock(&gLock);
        if (!gArmed) {
            // Wait for Java to hand us the Downloads path. Loading the library
            // without arming it (checkbox off) means we simply time out.
            struct timespec deadline {};
            clock_gettime(CLOCK_REALTIME, &deadline);
            deadline.tv_sec += kConfigTimeoutSec;
            while (!gArmed) {
                if (pthread_cond_timedwait(&gCond, &gLock, &deadline) != 0) break;
            }
        }
        if (gArmed) outDir = gOutDir;
        pthread_mutex_unlock(&gLock);
    }

    if (outDir.empty()) {
        LOGW("worker: never configured, nothing dumped");
        return nullptr;
    }
    RunDump(outDir);
    return nullptr;
}

void StartWorkerLocked() {
    if (gThreadStarted) return;
    gThreadStarted = true;
    pthread_t tid;
    if (pthread_create(&tid, nullptr, WorkerMain, nullptr) == 0) {
        pthread_detach(tid);
        LOGI("worker thread started");
    } else {
        gThreadStarted = false;
        LOGE("worker thread failed to start");
    }
}

}  // namespace

// ---------------------------------------------------------------- jni

extern "C" JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void*) {
    gVm = vm;  // cached for completeness; the worker makes no Java calls
    LOGI("JNI_OnLoad: arming worker");
    pthread_mutex_lock(&gLock);
    StartWorkerLocked();
    pthread_mutex_unlock(&gLock);
    return JNI_VERSION_1_6;
}

// The only Java -> native call: hand over the resolved output directory.
// Java loads this library (System.loadLibrary) only while the dumper checkbox
// is on, and only inside the guest process, so reaching here already means
// "dump this process".
extern "C" JNIEXPORT jboolean JNICALL
Java_top_niunaijun_blackboxa_util_PySdkDumper_nativeStart(
        JNIEnv* env, jobject /*thiz*/, jstring outDir) {
    const std::string dir = JStr(env, outDir);
    if (dir.empty()) {
        LOGE("nativeStart: empty output dir");
        return JNI_FALSE;
    }
    pthread_mutex_lock(&gLock);
    gOutDir = dir;
    gArmed = true;
    StartWorkerLocked();
    pthread_cond_signal(&gCond);
    pthread_mutex_unlock(&gLock);
    LOGI("nativeStart: out=%s", dir.c_str());
    return JNI_TRUE;
}
