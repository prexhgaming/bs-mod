// libbs_dumper.so — Python SDK dumper for the target game (CPython 3.11 title).
//
// The game statically links CPython into libGame.so, so the interpreter symbols
// are already mapped in the game process. We do NOT ship our own Python: we
// resolve what the game already has (dlopen/dlsym) and, when the dumper is
// enabled, serialize the real runtime layout into an SDK folder.
//
// Two paths are supported:
//   1. interpreter live  -> run a small Python script that writes sys.path,
//      sys.version, sys.prefix and copies site-packages / stdlib trees.
//   2. interpreter found but not initialised (dumper ran too early) -> write a
//      manifest from the on-disk lib/stdlib files so the SDK is still useful.

#include <jni.h>
#include <android/log.h>
#include <dlfcn.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstdio>
#include <cstring>
#include <fstream>
#include <string>
#include <vector>

#define LOG_TAG "BsSdkDumper"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

namespace {

constexpr size_t kMaxCopyDepth = 24;
constexpr size_t kMaxCopyFiles = 20000;
size_t gCopiedFiles = 0;

// ---------------------------------------------------------------- strings

std::string JStr(JNIEnv* env, jstring value) {
    if (value == nullptr) return {};
    const char* chars = env->GetStringUTFChars(value, nullptr);
    if (chars == nullptr) return {};
    std::string out(chars);
    env->ReleaseStringUTFChars(value, chars);
    return out;
}

bool EndsWith(const std::string& s, const std::string& suffix) {
    return s.size() >= suffix.size() &&
           s.compare(s.size() - suffix.size(), suffix.size(), suffix) == 0;
}

bool Exists(const std::string& path) {
    struct stat st {};
    return stat(path.c_str(), &st) == 0;
}

bool IsDir(const std::string& path) {
    struct stat st {};
    return stat(path.c_str(), &st) == 0 && S_ISDIR(st.st_mode);
}

bool EnsureDir(const std::string& path) {
    if (path.empty() || IsDir(path)) return true;
    const size_t slash = path.find_last_of('/');
    if (slash != std::string::npos && slash > 0) {
        if (!EnsureDir(path.substr(0, slash))) return false;
    }
    return mkdir(path.c_str(), 0775) == 0 || IsDir(path);
}

bool WriteFile(const std::string& path, const std::string& data) {
    std::ofstream out(path, std::ios::binary | std::ios::trunc);
    if (!out) return false;
    out.write(data.data(), static_cast<std::streamsize>(data.size()));
    return out.good();
}

std::string ReadTextFile(const std::string& path, size_t maxBytes = 4u << 20) {
    std::ifstream in(path, std::ios::binary);
    if (!in) return {};
    std::string data;
    data.resize(maxBytes);
    in.read(&data[0], static_cast<std::streamsize>(maxBytes));
    data.resize(static_cast<size_t>(in.gcount()));
    return data;
}

std::string JsonEscape(const std::string& in) {
    std::string out;
    out.reserve(in.size() + 8);
    for (char c : in) {
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
                    out += c;
                }
        }
    }
    return out;
}

// ---------------------------------------------------------------- fs walk

std::vector<std::string> ListDir(const std::string& dir) {
    std::vector<std::string> out;
    DIR* handle = opendir(dir.c_str());
    if (handle == nullptr) return out;
    while (dirent* entry = readdir(handle)) {
        const char* name = entry->d_name;
        if (strcmp(name, ".") == 0 || strcmp(name, "..") == 0) continue;
        out.emplace_back(name);
    }
    closedir(handle);
    return out;
}

bool CopyFileRaw(const std::string& from, const std::string& to) {
    if (gCopiedFiles >= kMaxCopyFiles) return false;
    if (!EnsureDir(to.substr(0, to.find_last_of('/')))) return false;

    std::ifstream src(from, std::ios::binary);
    if (!src) return false;
    std::ofstream dst(to, std::ios::binary | std::ios::trunc);
    if (!dst) return false;

    dst << src.rdbuf();
    if (dst.good()) {
        gCopiedFiles++;
        return true;
    }
    return false;
}

// Copies a directory tree, skipping entries that would explode the output.
void CopyTree(const std::string& from, const std::string& to, size_t depth = 0) {
    if (depth > kMaxCopyDepth || gCopiedFiles >= kMaxCopyFiles) return;
    if (!IsDir(from)) return;
    EnsureDir(to);

    for (const std::string& name : ListDir(from)) {
        if (name == "__pycache__") continue;              // regenerated at runtime
        if (name == ".git" || name == ".svn") continue;

        const std::string srcPath = from + "/" + name;
        const std::string dstPath = to + "/" + name;

        if (IsDir(srcPath)) {
            CopyTree(srcPath, dstPath, depth + 1);
        } else {
            CopyFileRaw(srcPath, dstPath);
        }
    }
}

// ------------------------------------------------------------- CPython API

// Minimal subset of the CPython C-API we resolve lazily from the game's process.
struct PyApi {
    void* handle = nullptr;

    const char* (*get_version)() = nullptr;
    int (*is_initialized)() = nullptr;
    void* (*gil_ensure)() = nullptr;
    void (*gil_release)(void*) = nullptr;
    int (*run_simple_string)(const char*) = nullptr;
    const char* (*get_path)() = nullptr;
    const char* (*get_prefix)() = nullptr;
    wchar_t* (*get_program_name)() = nullptr;

    bool HasInterpreter() const { return get_version != nullptr; }
    bool IsLive() const { return is_initialized != nullptr && is_initialized() != 0; }
};

template <typename T>
void Resolve(void* handle, const char* symbol, T& out) {
    if (out != nullptr) return;
    out = reinterpret_cast<T>(dlsym(handle, symbol));
}

// The interpreter lives inside the game's own libraries; try the game libs
// first, then fall back to a global lookup (the lib is already loaded by then).
PyApi ResolvePython(const std::string& nativeLibDir) {
    PyApi api;

    const char* candidates[] = {
        "libGame.so",
        "libpython3.11.so",
        "libpython3.11m.so",
        "libpython3.so",
        "libmain.so",
    };

    for (const char* name : candidates) {
        const std::string path = nativeLibDir + "/" + name;
        if (!Exists(path)) continue;

        void* handle = dlopen(path.c_str(), RTLD_NOW | RTLD_NOLOAD);
        if (handle == nullptr) handle = dlopen(path.c_str(), RTLD_NOW | RTLD_LOCAL);
        if (handle == nullptr) continue;

        Resolve(handle, "Py_GetVersion", api.get_version);
        if (api.get_version != nullptr) {
            api.handle = handle;
            LOGI("CPython symbols found in %s", name);
            break;
        }
        dlclose(handle);
    }

    // Nothing in the lib dir: maybe it is mapped under another soname.
    if (api.get_version == nullptr) {
        void* self = dlopen(nullptr, RTLD_NOW);
        if (self != nullptr) Resolve(self, "Py_GetVersion", api.get_version);
        if (api.get_version != nullptr) {
            api.handle = self;
            LOGI("CPython symbols found in the global namespace");
        }
    }

    if (api.get_version == nullptr) return api;

    void* handle = api.handle;
    Resolve(handle, "Py_IsInitialized", api.is_initialized);
    Resolve(handle, "PyGILState_Ensure", api.gil_ensure);
    Resolve(handle, "PyGILState_Release", api.gil_release);
    Resolve(handle, "PyRun_SimpleString", api.run_simple_string);
    Resolve(handle, "Py_GetPath", api.get_path);
    Resolve(handle, "Py_GetPrefix", api.get_prefix);
    Resolve(handle, "Py_GetProgramName", api.get_program_name);

    return api;
}

// Last resort: scrape a "Python 3.11.2" banner out of a library binary.
std::string ScanVersionBanner(const std::string& nativeLibDir) {
    const char* names[] = {"libGame.so", "libpython3.11.so", "libpython3.so"};
    for (const char* name : names) {
        const std::string path = nativeLibDir + "/" + name;
        const std::string blob = ReadTextFile(path, 32u << 20);
        if (blob.empty()) continue;

        const std::string needle = "Python 3.";
        const size_t at = blob.find(needle);
        if (at == std::string::npos) continue;

        std::string version;
        for (size_t i = at + needle.size() - 2;
             i < blob.size() && version.size() < 16; ++i) {
            const char c = blob[i];
            if ((c >= '0' && c <= '9') || c == '.') {
                version += c;
            } else {
                break;
            }
        }
        // "3.11.2" -> keep it only if it looks like a real version
        if (version.size() >= 3) return version;
    }
    return {};
}

// Python-side extraction. Writes the environment description and copies the
// importable trees that the game actually uses.
std::string BuildExtractScript(const std::string& outDir) {
    std::string script;
    script += "import os, sys, json, shutil\n";
    script += "out = r'''" + outDir + "'''\n";
    script += "os.makedirs(out, exist_ok=True)\n";
    script += "info = {\n";
    script += "  'version': sys.version,\n";
    script += "  'version_info': list(sys.version_info[:3]),\n";
    script += "  'executable': getattr(sys, 'executable', None),\n";
    script += "  'prefix': getattr(sys, 'prefix', None),\n";
    script += "  'base_prefix': getattr(sys, 'base_prefix', None),\n";
    script += "  'exec_prefix': getattr(sys, 'exec_prefix', None),\n";
    script += "  'platform': sys.platform,\n";
    script += "  'byteorder': sys.byteorder,\n";
    script += "  'maxsize': getattr(sys, 'maxsize', None),\n";
    script += "  'path': list(sys.path),\n";
    script += "  'modules': sorted(sys.modules.keys()),\n";
    script += "}\n";
    script += "with open(os.path.join(out, 'python_env.json'), 'w') as fh:\n";
    script += "  json.dump(info, fh, indent=2)\n";
    script += "copied = []\n";
    script += "for entry in list(sys.path):\n";
    script += "  if not entry or not os.path.isdir(entry):\n";
    script += "    continue\n";
    script += "  base = os.path.basename(entry.rstrip('/')) or 'root'\n";
    script += "  dest = os.path.join(out, 'modules', base)\n";
    script += "  try:\n";
    script += "    if os.path.exists(dest):\n";
    script += "      continue\n";
    script += "    shutil.copytree(entry, dest,\n";
    script += "      ignore=shutil.ignore_patterns('__pycache__', '*.pyc'))\n";
    script += "    copied.append({'src': entry, 'dst': dest})\n";
    script += "  except Exception as exc:\n";
    script += "    copied.append({'src': entry, 'error': str(exc)})\n";
    script += "with open(os.path.join(out, 'modules.json'), 'w') as fh:\n";
    script += "  json.dump(copied, fh, indent=2)\n";
    script += "print('bs-dumper: sdk written to', out)\n";
    return script;
}

// ------------------------------------------------------------ the natives

std::string DirOf(const std::string& path) {
    const size_t slash = path.find_last_of('/');
    return slash == std::string::npos ? std::string() : path.substr(0, slash);
}

std::string BaseOf(const std::string& path) {
    const size_t slash = path.find_last_of('/');
    return slash == std::string::npos ? path : path.substr(slash + 1);
}

// Locates stdlib archives/dirs shipped beside the game libraries or in its data
// directory, so path (2) of the dumper still produces something usable.
std::vector<std::string> FindStdlibRoots(const std::string& nativeLibDir,
                                         const std::string& dataDir,
                                         const std::string& sourceDir) {
    std::vector<std::string> roots;
    const std::string bases[] = {nativeLibDir, dataDir, DirOf(sourceDir),
                                 dataDir + "/files", dataDir + "/app_python"};

    for (const std::string& base : bases) {
        if (base.empty()) continue;
        for (const std::string& name : ListDir(base)) {
            const std::string full = base + "/" + name;
            const bool looksPython = EndsWith(name, ".zip") &&
                                     name.find("python") != std::string::npos;
            const bool isStdlibDir = IsDir(full) &&
                                     (name == "python3.11" || name == "stdlib" ||
                                      name == "site-packages" || name == "python");
            if (looksPython || isStdlibDir) roots.push_back(full);
        }
    }
    return roots;
}

}  // namespace

extern "C" {

JNIEXPORT jboolean JNICALL
Java_top_niunaijun_blackboxa_util_PySdkDumper_nativeDetect(
        JNIEnv* env, jobject /*thiz*/,
        jstring jNativeLibDir, jstring /*jDataDir*/, jstring /*jSourceDir*/) {

    const std::string nativeLibDir = JStr(env, jNativeLibDir);
    const PyApi api = ResolvePython(nativeLibDir);

    if (api.HasInterpreter()) {
        const char* version = api.get_version();
        LOGI("detect: interpreter found (%s), live=%d",
             version != nullptr ? version : "?", api.IsLive() ? 1 : 0);
        return JNI_TRUE;
    }

    const std::string banner = ScanVersionBanner(nativeLibDir);
    if (!banner.empty()) {
        LOGI("detect: version banner %s (entry points not resolvable yet)",
             banner.c_str());
        return JNI_TRUE;
    }

    LOGW("detect: no CPython runtime in %s", nativeLibDir.c_str());
    return JNI_FALSE;
}

JNIEXPORT jstring JNICALL
Java_top_niunaijun_blackboxa_util_PySdkDumper_nativeVersion(
        JNIEnv* env, jobject /*thiz*/) {

    void* self = dlopen(nullptr, RTLD_NOW);
    PyApi probe{};
    if (self != nullptr) Resolve(self, "Py_GetVersion", probe.get_version);

    if (probe.get_version == nullptr) {
        return env->NewStringUTF("");
    }

    const char* raw = probe.get_version();
    if (raw == nullptr) return env->NewStringUTF("");

    // "3.11.2 (main, ...)" -> "3.11.2"
    std::string version;
    for (const char* p = raw; *p != '\0' && version.size() < 16; ++p) {
        if ((*p >= '0' && *p <= '9') || *p == '.') {
            version += *p;
        } else {
            break;
        }
    }
    return env->NewStringUTF(version.c_str());
}

JNIEXPORT jstring JNICALL
Java_top_niunaijun_blackboxa_util_PySdkDumper_nativeDump(
        JNIEnv* env, jobject /*thiz*/,
        jstring jOutDir, jstring jNativeLibDir,
        jstring jDataDir, jstring jSourceDir) {

    const std::string outDir = JStr(env, jOutDir);
    const std::string nativeLibDir = JStr(env, jNativeLibDir);
    const std::string dataDir = JStr(env, jDataDir);
    const std::string sourceDir = JStr(env, jSourceDir);

    if (outDir.empty()) {
        LOGE("dump: no output directory");
        return env->NewStringUTF("");
    }
    if (!EnsureDir(outDir)) {
        LOGE("dump: cannot create %s", outDir.c_str());
        return env->NewStringUTF("");
    }

    gCopiedFiles = 0;
    const PyApi api = ResolvePython(nativeLibDir);

    bool scriptRan = false;
    if (api.HasInterpreter() && api.IsLive() && api.run_simple_string != nullptr) {
        void* gil = (api.gil_ensure != nullptr) ? api.gil_ensure() : nullptr;
        const std::string script = BuildExtractScript(outDir);
        const std::string scriptPath = outDir + "/.bs_extract.py";
        if (WriteFile(scriptPath, script)) {
            LOGI("dump: running in-process extractor");
            const std::string call = "exec(open(r'''" + scriptPath + "''').read())";
            scriptRan = api.run_simple_string(call.c_str()) == 0;
            unlink(scriptPath.c_str());
        }
        if (gil != nullptr && api.gil_release != nullptr) api.gil_release(gil);
    }

    // Always emit a manifest describing what we could see from here.
    const std::vector<std::string> roots =
            FindStdlibRoots(nativeLibDir, dataDir, sourceDir);

    std::string manifest;
    manifest += "{\n";
    manifest += "  \"generated_by\": \"bs-dumper\",\n";
    manifest += "  \"package_source\": \"" + JsonEscape(BaseOf(sourceDir)) + "\",\n";
    manifest += "  \"native_lib_dir\": \"" + JsonEscape(nativeLibDir) + "\",\n";
    manifest += "  \"data_dir\": \"" + JsonEscape(dataDir) + "\",\n";
    manifest += "  \"interpreter_live\": ";
    manifest += (api.HasInterpreter() && api.IsLive()) ? "true,\n" : "false,\n";
    manifest += "  \"in_process_extract\": ";
    manifest += scriptRan ? "true,\n" : "false,\n";

    std::string version;
    if (api.HasInterpreter() && api.get_version() != nullptr) {
        version = api.get_version();
    } else {
        version = ScanVersionBanner(nativeLibDir);
    }
    manifest += "  \"python_version\": \"" + JsonEscape(version) + "\",\n";

    if (api.get_prefix != nullptr) {
        const char* prefix = api.get_prefix();
        if (prefix != nullptr) {
            manifest += "  \"prefix\": \"" + JsonEscape(prefix) + "\",\n";
        }
    }
    if (api.get_path != nullptr) {
        const char* path = api.get_path();
        if (path != nullptr) {
            manifest += "  \"sys_path_raw\": \"" + JsonEscape(path) + "\",\n";
        }
    }

    manifest += "  \"stdlib_roots\": [";
    for (size_t i = 0; i < roots.size(); ++i) {
        manifest += (i == 0 ? "\n" : ",\n");
        manifest += "    \"" + JsonEscape(roots[i]) + "\"";
    }
    manifest += roots.empty() ? "],\n" : "\n  ],\n";

    manifest += "  \"native_libraries\": [";
    bool firstLib = true;
    for (const std::string& name : ListDir(nativeLibDir)) {
        if (!EndsWith(name, ".so")) continue;
        manifest += firstLib ? "\n" : ",\n";
        firstLib = false;
        manifest += "    \"" + JsonEscape(name) + "\"";
    }
    manifest += firstLib ? "]\n" : "\n  ]\n";
    manifest += "}\n";

    WriteFile(outDir + "/sdk_manifest.json", manifest);

    // Copy the stdlib roots we found (archive or directory) when the live
    // extractor could not run — that is what makes the SDK portable.
    if (!scriptRan) {
        for (const std::string& root : roots) {
            const std::string dest = outDir + "/modules/" + BaseOf(root);
            if (IsDir(root)) {
                CopyTree(root, dest);
            } else {
                CopyFileRaw(root, dest);
            }
        }
    }

    LOGI("dump: finished (%zu files copied) -> %s", gCopiedFiles, outDir.c_str());

    std::string report;
    report += "bs-dumper report\n";
    report += "output:            " + outDir + "\n";
    report += "python version:    " + version + "\n";
    report += "interpreter live:  ";
    report += (api.HasInterpreter() && api.IsLive()) ? "yes\n" : "no\n";
    report += "in-process dump:   ";
    report += scriptRan ? "yes\n" : "no\n";
    report += "files copied:      " + std::to_string(gCopiedFiles) + "\n";
    report += "stdlib roots:      " + std::to_string(roots.size()) + "\n";
    WriteFile(outDir + "/REPORT.txt", report);

    return env->NewStringUTF(outDir.c_str());
}

}  // extern "C"
