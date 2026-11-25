// py_runtime_dump.cpp — see py_runtime_dump.hpp for the rationale.
//
// Design notes:
//   * Nothing is executed and nothing is hooked. The only "handle" into the
//     target is a read-only fd on /proc/self/mem, which is always available to
//     the process itself.
//   * A .pyc is carved at its exact length by walking the marshalled code
//     object. A fixed-size guess would produce files CPython refuses to load,
//     which defeats the point of a dumper.
//   * Every read is bounds-checked against the mapping and against a hard
//     limit, so a corrupt or hostile region can only cost us a failed carve.

#include "py_runtime_dump.hpp"

#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#include <algorithm>
#include <cerrno>
#include <cstdio>
#include <cstring>
#include <set>
#include <sstream>

namespace pydump {
namespace {

// ------------------------------------------------------------------ limits

constexpr uint64_t kChunk = 4ull * 1024 * 1024;      // scan window
constexpr uint64_t kOverlap = 64;                    // magic may straddle
constexpr size_t kMaxString = 16u * 1024u * 1024u;   // marshal string bound
constexpr uint32_t kMaxSeq = 1000000;                // marshal container bound
constexpr int kMaxDepth = 60;                        // marshal recursion bound
constexpr size_t kMaxLine = 4096;                    // source line sanity
// Ref-table bound: one slot per FLAG_REF object in the stream. A .pyc is
// capped at maxPycBytes, so this only guards against a corrupt count.
constexpr size_t kMaxRefs = 4u * 1024u * 1024u;
constexpr size_t kNoSlot = static_cast<size_t>(-1);

// ------------------------------------------------------------------ logging

void NoopLog(const std::string&) {}

// ------------------------------------------------------------------ fs

std::string Join(const std::string& a, const std::string& b) {
    if (a.empty()) return b;
    if (a.back() == '/') return a + b;
    return a + "/" + b;
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

bool WriteBytes(const std::string& path, const char* data, size_t n) {
    FILE* f = fopen(path.c_str(), "wb");
    if (f == nullptr) return false;
    const bool ok = n == 0 || fwrite(data, 1, n, f) == n;
    fclose(f);
    return ok;
}

bool WriteText(const std::string& path, const std::string& text) {
    return WriteBytes(path, text.data(), text.size());
}

// rel paths mirror the game's package tree (gclient/gameplay/space.pyc), so
// the parent directories have to exist before fopen() will succeed.
bool MakeParentDirs(const std::string& path) {
    const size_t slash = path.find_last_of('/');
    if (slash == std::string::npos || slash == 0) return true;
    return MakeDirs(path.substr(0, slash));
}

// 64-bit FNV-1a. Used to spot the same module sitting in several mappings;
// carved source has no name to key on, so its content is the identity.
uint64_t Fnv1a(const std::string& s) {
    uint64_t h = 1469598103934665603ull;
    for (unsigned char c : s) {
        h ^= c;
        h *= 1099511628211ull;
    }
    return h;
}

std::string HexLower(uint64_t v) {
    char buf[32];
    snprintf(buf, sizeof(buf), "%llx", static_cast<unsigned long long>(v));
    return buf;
}

// "gclient.gameplay.util.replay_util" -> "gclient/gameplay/util/replay_util"
std::string ModuleToPath(const std::string& module) {
    std::string out;
    out.reserve(module.size());
    for (char c : module) {
        if (c == '.') {
            out += '/';
        } else if (isalnum(static_cast<unsigned char>(c)) || c == '_' || c == '/') {
            out += c;
        } else {
            out += '_';
        }
    }
    return out;
}

std::string SafeName(const std::string& raw);

// Turn a marshalled co_filename (a full on-device path like
// ".../Package/Script/Python/gclient/gameplay/util/replay_util.py") into a
// relative, filesystem-safe path ("gclient/gameplay/util/replay_util").
// Falls back to ModuleToPath() when the path has no recognisable anchor.
std::string FilenameToRelPath(const std::string& filename) {
    std::string rel = filename;
    if (rel.empty()) return rel;

    // Anchor on the last "Python/" component so we drop the sandbox prefix.
    const std::string anchor = "Python/";
    const size_t at = rel.rfind(anchor);
    if (at != std::string::npos) {
        rel = rel.substr(at + anchor.size());
    } else {
        // No anchor: keep only the basename to stay safe.
        const size_t slash = rel.find_last_of('/');
        if (slash != std::string::npos) rel = rel.substr(slash + 1);
    }

    // Drop the source extension; the caller appends its own.
    for (const char* ext : {".pyc", ".pyo", ".pyi", ".py"}) {
        const size_t n = strlen(ext);
        if (rel.size() > n && rel.compare(rel.size() - n, n, ext) == 0) {
            rel.resize(rel.size() - n);
            break;
        }
    }

    // A dotted module name (no slashes) still maps through ModuleToPath.
    if (rel.find('/') == std::string::npos && rel.find('.') != std::string::npos) {
        rel = ModuleToPath(rel);
    }
    rel = SafeName(rel);
    while (!rel.empty() && rel.front() == '/') rel.erase(rel.begin());
    return rel;
}

// Keep only characters that are safe in a file name, and bound the length.
std::string SafeName(const std::string& raw) {
    std::string out;
    out.reserve(raw.size());
    for (char c : raw) {
        const unsigned char u = static_cast<unsigned char>(c);
        if (isalnum(u) || c == '_' || c == '-' || c == '.' || c == '/') {
            out += c;
        } else {
            out += '_';
        }
    }
    if (out.size() > 180) out.resize(180);
    return out;
}

// ------------------------------------------------------------------ regions

struct Region {
    uintptr_t start = 0;
    uintptr_t end = 0;
    std::string perms;
    std::string path;
};

std::vector<Region> ReadRegions() {
    std::vector<Region> out;
    FILE* f = fopen("/proc/self/maps", "rb");
    if (f == nullptr) return out;

    char line[1024];
    while (fgets(line, sizeof(line), f) != nullptr) {
        Region r;
        unsigned long long s = 0, e = 0;
        char perms[8] = {0};
        // "start-end perms offset dev inode path"
        if (sscanf(line, "%llx-%llx %7s", &s, &e, perms) != 3) continue;
        r.start = static_cast<uintptr_t>(s);
        r.end = static_cast<uintptr_t>(e);
        r.perms = perms;

        const char* slash = strchr(line, '/');
        if (slash != nullptr) {
            r.path = slash;
            while (!r.path.empty() &&
                   (r.path.back() == '\n' || r.path.back() == '\r' ||
                    r.path.back() == ' ')) {
                r.path.pop_back();
            }
        }
        if (r.end > r.start) out.push_back(r);
    }
    fclose(f);
    return out;
}

bool Interesting(const Region& r) {
    if (r.perms.empty() || r.perms[0] != 'r') return false;  // readable only
    if (r.path.find("libbs_dumper.so") != std::string::npos) return false;

    static const char* kSkip[] = {
            "/system/",     "/apex/",      "/vendor/",   "/data/dalvik-cache",
            "/dev/",        "/sys/",       "/proc/",     "/data/misc/",
            "libdobby",     "libxdl",      "libkitty",   "/data/user/0/",
    };
    for (const char* s : kSkip) {
        if (r.path.find(s) != std::string::npos) return false;
    }
    return true;
}

// ------------------------------------------------------------------ memory

class Mem {
public:
    Mem() : fd_(-1) {}
    ~Mem() {
        if (fd_ >= 0) close(fd_);
    }

    bool Open() {
        fd_ = open("/proc/self/mem", O_RDONLY);
        return fd_ >= 0;
    }

    // Reads exactly n bytes; returns false on any short read.
    bool Read(uintptr_t addr, void* buf, size_t n) const {
        if (fd_ < 0 || n == 0) return n == 0;
        char* p = static_cast<char*>(buf);
        size_t done = 0;
        while (done < n) {
            const ssize_t got = pread(fd_, p + done, n - done,
                                      static_cast<off_t>(addr + done));
            if (got <= 0) return false;
            done += static_cast<size_t>(got);
        }
        return true;
    }

    bool U8(uintptr_t addr, uint8_t* v) const { return Read(addr, v, 1); }

    bool U32(uintptr_t addr, uint32_t* v) const { return Read(addr, v, 4); }

    // Best-effort read of a whole scan window. Always fills n bytes -- holes
    // are zero-filled, and a zero can never start a magic match or a source
    // line -- and returns how many bytes came from real memory.
    //
    // This must be per-page tolerant. /proc/self/maps reports merged VMAs, so
    // a "region" routinely spans pages that pread() refuses with EIO (guard
    // pages, freshly unmapped tails). Aborting on the first failure discarded
    // windows that were almost entirely readable, which silently lost every
    // .pyc whose slab happened to sit next to such a hole.
    size_t ReadWindow(uintptr_t addr, void* buf, size_t n, size_t pageSize) const {
        char* p = static_cast<char*>(buf);
        if (fd_ < 0 || n == 0) return 0;
        size_t done = 0;
        size_t real = 0;
        while (done < n) {
            const ssize_t got = pread(fd_, p + done, n - done,
                                      static_cast<off_t>(addr + done));
            if (got > 0) {
                done += static_cast<size_t>(got);
                real += static_cast<size_t>(got);
                continue;
            }
            // Unreadable page: blank it and step over exactly one page.
            size_t hole = std::min(pageSize, n - done);
            if (hole == 0) hole = 1;
            std::memset(p + done, 0, hole);
            done += hole;
        }
        return real;
    }

private:
    int fd_;
};

// ------------------------------------------------------------------ marshal

struct SkipResult {
    uint64_t consumed = 0;
};

// Walks one marshalled object without materialising it. Only the strings the
// caller asks for (co_filename / co_name) are copied out.
class Marshal {
public:
    Marshal(Mem* mem, uintptr_t pos, uintptr_t limit, int major, int minor)
        : mem_(mem), p_(pos), limit_(limit) {
        (void)major;
        (void)minor;
    }

    // Walks one marshalled object. `typeOut`, when given, receives the type
    // marker that was consumed (needed to spot the dict's TYPE_NULL terminator).
    bool Skip(SkipResult* out, int depth, uint8_t* typeOut = nullptr) {
        if (depth > kMaxDepth) return false;
        if (p_ >= limit_) return false;

        uint8_t raw = 0;
        if (!mem_->U8(p_, &raw)) return false;
        const uintptr_t start = p_;
        ++p_;

        // FLAG_REF (0x80) marks an object the marshaller adds to its ref table.
        // A repeat is then written as a 4-byte TYPE_REF index into that table.
        // co_filename is usually a REF (nested code objects emit the same
        // string first), so the name is only recoverable if the table is
        // rebuilt as we walk.
        const bool tracked = (raw & 0x80) != 0;
        const uint8_t type = (uint8_t)(raw & 0x7F);
        if (typeOut != nullptr) *typeOut = type;

        // Slot must be reserved before recursing: the marshaller appends the
        // parent to the table before its children.
        size_t slot = kNoSlot;
        if (tracked && refs_.size() < kMaxRefs) {
            slot = refs_.size();
            refs_.push_back(std::string());
        }

        switch (type) {
            case '0':  // NULL
            case 'N':  // None
            case 'F':  // False
            case 'T':  // True
            case 'S':  // StopIteration
            case '.':  // Ellipsis
                break;

            case 'i':  // int32
                p_ += 4;
                break;

            case 'g':  // binary float (8 bytes, little-endian double)
                p_ += 8;
                break;

            case 'f': {  // text float: 1-byte length + digits
                uint8_t n = 0;
                if (!mem_->U8(p_, &n)) return false;
                ++p_;
                p_ += n;
                break;
            }

            case 'I':  // int64
                p_ += 8;
                break;

            case 'y':  // binary complex
                p_ += 16;
                break;

            case 'x': {  // text complex: two short pstrings
                for (int k = 0; k < 2; ++k) {
                    uint8_t n = 0;
                    if (!mem_->U8(p_, &n)) return false;
                    ++p_;
                    p_ += n;
                }
                break;
            }

            case 'l': {  // long: int32 digit count (signed), 2 bytes per digit
                uint32_t n = 0;
                if (!mem_->U32(p_, &n)) return false;
                p_ += 4;
                const int64_t sn = static_cast<int64_t>(static_cast<int32_t>(n));
                const int64_t digits = sn < 0 ? -sn : sn;
                p_ += static_cast<uintptr_t>(digits * 2);
                break;
            }

            case 's':  // string (also how bytes/bytearray are written)
            case 't':  // interned
            case 'u':  // unicode
            case 'a':  // ascii
            case 'A':  // ascii interned
            {
                uint32_t n = 0;
                if (!mem_->U32(p_, &n)) return false;
                p_ += 4;
                if (n > kMaxString) return false;
                if (capture_ != nullptr || slot != kNoSlot) {
                    std::string s;
                    s.resize(n);
                    if (n > 0 && !mem_->Read(p_, &s[0], n)) return false;
                    if (slot != kNoSlot) refs_[slot] = s;
                    if (capture_ != nullptr) *capture_ = s;
                }
                p_ += n;
                break;
            }

            case 'z':  // short ascii
            case 'Z':  // short ascii interned
            {
                uint8_t n = 0;
                if (!mem_->U8(p_, &n)) return false;
                ++p_;
                if (capture_ != nullptr || slot != kNoSlot) {
                    std::string s;
                    s.resize(n);
                    if (n > 0 && !mem_->Read(p_, &s[0], n)) return false;
                    if (slot != kNoSlot) refs_[slot] = s;
                    if (capture_ != nullptr) *capture_ = s;
                }
                p_ += n;
                break;
            }

            case 'r': {  // ref
                // w_long(): the ref index is 4 bytes, not one. A wrong size
                // here desyncs the stream and every later field lies.
                uint32_t idx = 0;
                if (!mem_->U32(p_, &idx)) return false;
                p_ += 4;
                if (capture_ != nullptr && idx < refs_.size()) {
                    *capture_ = refs_[idx];
                }
                break;
            }

            case '(':  // tuple
            case '[':  // list
            case '<':  // set
            case '>':  // frozenset
            {
                uint32_t n = 0;
                if (!mem_->U32(p_, &n)) return false;
                p_ += 4;
                if (n > kMaxSeq) return false;
                for (uint32_t i = 0; i < n; ++i) {
                    if (!Skip(nullptr, depth + 1)) return false;
                }
                break;
            }

            case ')': {  // small tuple
                uint8_t n = 0;
                if (!mem_->U8(p_, &n)) return false;
                ++p_;
                for (uint8_t i = 0; i < n; ++i) {
                    if (!Skip(nullptr, depth + 1)) return false;
                }
                break;
            }

            case '{': {  // dict: key/value pairs, ended by a TYPE_NULL marker
                // Follows r_object()'s loop exactly: read a key, stop when it
                // is TYPE_NULL, otherwise read its value.
                for (;;) {
                    uint8_t kt = 0;
                    if (!Skip(nullptr, depth + 1, &kt)) return false;
                    if (kt == '0') break;
                    if (!Skip(nullptr, depth + 1)) return false;
                }
                break;
            }

            case 'c': {  // code object, CPython 3.11/3.12 layout
                // w_complex_object(TYPE_CODE) writes:
                //   5 raw w_long() values with NO type byte
                //     argcount, posonlyargcount, kwonlyargcount,
                //     stacksize, flags
                //   then, at field index 5..15:
                //     5 co_code        6 co_consts     7 co_names
                //     8 localsplusnames  9 localspluskinds
                //    10 co_filename   11 co_name      12 co_qualname
                //    13 co_firstlineno  <- another raw w_long
                //    14 co_linetable  15 co_exceptiontable
                // Verified against CPython v3.11.2 and v3.12.0 marshal.c.
                for (int i = 0; i < 5; ++i) p_ += 4;

                // Only the outermost code object names the module. Nested ones
                // (functions/classes inside co_consts) carry the same
                // co_filename but their own co_name, and they are walked first,
                // so capturing unconditionally left `module` = "helper".
                const bool outer = (depth == 0);

                for (int i = 5; i < 16; ++i) {
                    if (i == 13) {  // co_firstlineno: raw w_long, no marker
                        p_ += 4;
                        continue;
                    }
                    std::string* cap = nullptr;
                    if (outer && i == 10) {
                        cap = &filename_;
                    } else if (outer && i == 11) {
                        cap = &name_;
                    }
                    capture_ = cap;
                    if (!Skip(nullptr, depth + 1)) return false;
                }
                capture_ = nullptr;
                break;
            }

            default:
                return false;  // unknown type: not a code object we can size
        }

        if (p_ > limit_) return false;
        if (out != nullptr) out->consumed = p_ - start;
        return true;
    }

    const std::string& filename() const { return filename_; }
    const std::string& name() const { return name_; }

private:
    Mem* mem_;
    uintptr_t p_;
    uintptr_t limit_;
    // Rebuilt marshal ref table: index -> string, empty for non-string slots.
    std::vector<std::string> refs_;
    std::string* capture_ = nullptr;
    std::string filename_;
    std::string name_;
};

// ------------------------------------------------------------------ magic

struct Magic {
    uint32_t value;  // little-endian read of the 4 magic bytes
    int major;
    int minor;
};

// CPython's importlib._bootstrap_external.MAGIC_NUMBER, as stored on disk.
const Magic kMagics[] = {
        {0x0A0D0DF3u, 3, 13},  // F3 0D 0D 0A
        {0x0A0D0DCBu, 3, 12},  // CB 0D 0D 0A
        {0x0A0D0DA7u, 3, 11},  // A7 0D 0D 0A
        {0x0A0D0D6Fu, 3, 10},  // 6F 0D 0D 0A
        {0x0A0D0D61u, 3, 9},   // 61 0D 0D 0A
        {0x0A0D0D55u, 3, 8},   // 55 0D 0D 0A
        {0x0A0D0D42u, 3, 7},   // 42 0D 0D 0A
};

const Magic* MatchMagic(uint32_t v) {
    for (const Magic& m : kMagics) {
        if (m.value == v) return &m;
    }
    return nullptr;
}

// ------------------------------------------------------------------ carving

bool CarvePyc(Mem* mem, uintptr_t addr, uintptr_t limit, const Magic& magic,
              uint64_t maxBytes, std::string* blob, Found* out) {
    // 16-byte header: magic(4) flags(4) mtime(4) size(4)
    uint8_t header[16];
    if (!mem->Read(addr, header, sizeof(header))) return false;

    Marshal m(mem, addr + sizeof(header), limit, magic.major, magic.minor);
    SkipResult sr;
    if (!m.Skip(&sr, 0)) return false;

    const uint64_t total = sizeof(header) + sr.consumed;
    if (total < 32 || total > maxBytes) return false;
    if (addr + total > limit) return false;

    blob->resize(static_cast<size_t>(total));
    if (!mem->Read(addr, &(*blob)[0], blob->size())) return false;

    out->kind = "pyc";
    out->module = m.name();
    out->filename = m.filename();
    out->address = addr;
    out->size = total;
    out->confidence = 95;
    return true;
}

bool LooksLikeSourceStart(const std::string& line) {
    static const char* kPrefixes[] = {
            "import ",      "from ",        "def ",         "class ",
            "async def ",   "@",            "#!",           "# -*-",
            "if __name__",  "\"\"\"",        "'''",
    };
    for (const char* p : kPrefixes) {
        if (line.compare(0, strlen(p), p) == 0) return true;
    }
    return false;
}

// Bytes that can legitimately appear inside a UTF-8 Python source file.
// Anything else (NUL, other control codes, a stray 0x7F) ends a text run --
// this is what stops a carve from walking out of one file and into whatever
// unrelated memory happens to follow it.
bool IsSourceByte(uint8_t c) {
    if (c == '\t' || c == '\n' || c == '\r' || c == '\f') return true;
    if (c >= 0x20 && c <= 0x7E) return true;
    return c >= 0x80;  // UTF-8 lead/continuation
}

// True when the first non-blank line of `head` looks like Python. Blank and
// indented-leading lines are skipped so a file that starts with an empty line
// still qualifies.
bool LooksLikePythonHead(const char* p, size_t n) {
    size_t i = 0;
    while (i < n) {
        size_t e = i;
        while (e < n && p[e] != '\n') ++e;
        size_t s = i;
        while (s < e && (p[s] == ' ' || p[s] == '\t' || p[s] == '\r')) ++s;
        if (s < e) {
            return LooksLikeSourceStart(std::string(p + s, e - s));
        }
        i = e + 1;
    }
    return false;
}

// Carves one run of source text starting at `start`, bounded by `limit`.
// The run ends at the first byte that cannot occur in Python source.
bool CarveSource(Mem* mem, uintptr_t start, uintptr_t limit, uint64_t maxBytes,
                 std::string* text, Found* out) {
    text->clear();
    text->reserve(4096);
    uintptr_t p = start;
    bool sawDef = false, sawClass = false, sawImport = false;

    while (p < limit && text->size() < maxBytes) {
        std::string line;
        bool gotAny = false;
        bool endOfRun = false;
        while (p < limit && line.size() < kMaxLine) {
            uint8_t c = 0;
            if (!mem->U8(p, &c)) break;
            ++p;
            gotAny = true;
            if (c == '\n') break;
            if (c == '\r') continue;
            if (!IsSourceByte(c)) {
                endOfRun = true;
                break;
            }
            line += static_cast<char>(c);
        }
        if (endOfRun) break;
        if (!gotAny) break;

        if (line.compare(0, 4, "def ") == 0 || line.compare(0, 10, "async def ") == 0) {
            sawDef = true;
        } else if (line.compare(0, 6, "class ") == 0) {
            sawClass = true;
        } else if (line.compare(0, 7, "import ") == 0 ||
                   line.compare(0, 5, "from ") == 0) {
            sawImport = true;
        }

        *text += line;
        *text += '\n';
    }

    // A real module is never a couple of lines; and it has to contain at least
    // one def/class/import or it is just prose that happened to survive.
    if (text->size() < 64) return false;
    if (!sawDef && !sawClass && !sawImport) return false;

    int confidence = 45;
    if (sawDef) confidence += 20;
    if (sawClass) confidence += 15;
    if (sawImport) confidence += 15;

    out->kind = "py";
    out->module.clear();
    out->filename.clear();
    out->address = start;
    out->size = text->size();
    out->confidence = confidence > 100 ? 100 : confidence;

    return true;
}

// ------------------------------------------------------------------ index

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

std::string BuildIndex(const Options& opt, const Result& r) {
    std::string j;
    j += "{\n";
    j += "  \"tool\": \"py_runtime_dump\",\n";
    j += "  \"game\": \"" + JsonEscape(opt.gamePkg) + "\",\n";
    j += "  \"python\": \"" + JsonEscape(opt.pythonVersion) + "\",\n";
    j += "  \"regions\": " + std::to_string(r.regions) + ",\n";
    j += "  \"scannedBytes\": " + std::to_string(r.scanned) + ",\n";
    j += "  \"pyc\": " + std::to_string(r.pyc) + ",\n";
    j += "  \"source\": " + std::to_string(r.src) + ",\n";
    j += "  \"failed\": " + std::to_string(r.failed) + ",\n";
    j += "  \"files\": [\n";
    for (size_t i = 0; i < r.found.size(); ++i) {
        const Found& f = r.found[i];
        j += "    {\"kind\": \"" + JsonEscape(f.kind) + "\", ";
        j += "\"module\": \"" + JsonEscape(f.module) + "\", ";
        j += "\"filename\": \"" + JsonEscape(f.filename) + "\", ";
        j += "\"address\": \"" + HexLower(f.address) + "\", ";
        j += "\"size\": " + std::to_string(f.size) + ", ";
        j += "\"confidence\": " + std::to_string(f.confidence) + ", ";
        j += "\"out\": \"" + JsonEscape(f.outPath) + "\"}";
        j += (i + 1 == r.found.size()) ? "\n" : ",\n";
    }
    j += "  ]\n}\n";
    return j;
}

}  // namespace

// ------------------------------------------------------------------ driver

Result Run(const Options& opt) {
    Result r;
    const std::function<void(const std::string&)> log = opt.log ? opt.log : NoopLog;

    const std::string root = Join(opt.outDir, "runtime");
    const std::string pycDir = Join(root, "pyc");
    const std::string srcDir = Join(root, "src");
    if (!MakeDirs(pycDir) || !MakeDirs(srcDir)) {
        log("pydump: cannot create output dirs under " + root);
        return r;
    }

    Mem mem;
    if (!mem.Open()) {
        log("pydump: cannot open /proc/self/mem");
        return r;
    }

    const std::vector<Region> regions = ReadRegions();
    log("pydump: " + std::to_string(regions.size()) + " mappings");

    uint64_t scanned = 0;
    bool capped = false;
    // /proc/self/mem fails per page, not per region.
    const long ps = sysconf(_SC_PAGESIZE);
    const size_t pageSize = ps > 0 ? static_cast<size_t>(ps) : 4096;

    // The same module is often resident several times (heap copy, mmap copy,
    // decompression buffer). Keying on the output path keeps the dump unique.
    std::set<std::string> writtenPaths;
    // Carved source has no name, so identical content in two mappings is
    // recognised by hash instead of path.
    std::set<uint64_t> writtenHashes;
    // Source carving is heuristic, so a candidate that starts inside an
    // already-carved span is a duplicate fragment, not a new file.
    std::vector<std::pair<uintptr_t, uintptr_t>> srcCovered;

    auto insideCovered = [&srcCovered](uintptr_t a) {
        for (const std::pair<uintptr_t, uintptr_t>& r : srcCovered) {
            if (a >= r.first && a < r.second) return true;
        }
        return false;
    };

    for (const Region& region : regions) {
        if (capped) break;
        if (!Interesting(region)) continue;

        const uint64_t size = region.end - region.start;
        if (size == 0) continue;
        r.regions += 1;

        const uint64_t span = std::min<uint64_t>(size, opt.maxRegionBytes);
        uint64_t pos = 0;

        while (pos < span) {
            if (scanned >= opt.maxTotalBytes) {
                capped = true;
                break;
            }
            const uint64_t want =
                    std::min<uint64_t>(kChunk, span - pos);
            const uint64_t budget = opt.maxTotalBytes - scanned;
            const uint64_t take = std::min(want, budget);
            if (take == 0) {
                capped = true;
                break;
            }

            std::vector<char> buf(static_cast<size_t>(take + kOverlap));
            // Tolerant window read: one unreadable page must not discard the
            // rest of the window.
            mem.ReadWindow(region.start + pos, buf.data(),
                           static_cast<size_t>(take), pageSize);
            scanned += take;

            const size_t len = static_cast<size_t>(take);
            for (size_t i = 0; i + 4 <= len; ++i) {
                const uintptr_t at = region.start + pos + i;

                if (opt.scanPyc && r.pyc < opt.maxPyc) {
                    uint32_t v = 0;
                    memcpy(&v, buf.data() + i, 4);
                    const Magic* magic = MatchMagic(v);
                    if (magic != nullptr) {
                        Found f;
                        std::string blob;
                        if (CarvePyc(&mem, at, region.end, *magic, opt.maxPycBytes,
                                     &blob, &f)) {
                            // "…/Python/gclient/gameplay/space.py" becomes
                            // "gclient/gameplay/space.pyc". Falls back to an
                            // address-based name when nothing was captured.
                            std::string rel = FilenameToRelPath(f.filename);
                            if (rel.empty()) {
                                rel = "pyc_" + HexLower(at) + "_" +
                                      std::to_string(f.size);
                            }
                            if (rel.size() < 4 ||
                                rel.compare(rel.size() - 4, 4, ".pyc") != 0) {
                                rel += ".pyc";
                            }
                            f.outPath = Join(pycDir, rel);
                            if (!writtenPaths.insert(f.outPath).second) {
                                // Already dumped from another mapping.
                            } else if (MakeParentDirs(f.outPath) &&
                                       WriteBytes(f.outPath, blob.data(), blob.size())) {
                                r.pyc += 1;
                                r.found.push_back(f);
                                log("pydump: pyc @" + HexLower(at) + " " +
                                    std::to_string(f.size) + "B " +
                                    (f.filename.empty() ? "?" : f.filename));
                            } else {
                                r.failed += 1;
                            }
                        }
                        i += 3;
                        continue;
                    }
                }

                if (opt.scanSource && r.src < opt.maxSrc) {
                    // Fire only at the START of a run of source bytes, so each
                    // embedded file is carved once from its real beginning
                    // rather than once per source-looking line inside it.
                    const uint8_t b0 = static_cast<uint8_t>(buf[i]);
                    const bool runStart =
                            IsSourceByte(b0) &&
                            (i == 0 || !IsSourceByte(static_cast<uint8_t>(buf[i - 1])));
                    if (runStart) {
                        const size_t head = std::min<size_t>(128, len - i);
                        if (LooksLikePythonHead(buf.data() + i, head)) {
                            const uintptr_t srcAt = region.start + pos + i;
                            if (!insideCovered(srcAt)) {
                                Found f;
                                std::string text;
                                if (CarveSource(&mem, srcAt, region.end,
                                                opt.maxSrcBytes, &text, &f)) {
                                    srcCovered.push_back(
                                            {srcAt, srcAt + text.size()});
                                    if (!writtenHashes.insert(Fnv1a(text)).second) {
                                        continue;  // same file, other mapping
                                    }
                                    const std::string guess =
                                            f.filename.empty()
                                                    ? ("src_" + HexLower(srcAt))
                                                    : f.filename;
                                    std::string rel = FilenameToRelPath(guess);
                                    if (rel.empty()) rel = SafeName(guess);
                                    if (rel.empty()) rel = "src_" + HexLower(srcAt);
                                    // Suffix test must look at the END. The old
                                    // code compared the first 3 chars, so
                                    // "space.py" became "space.py.py".
                                    if (rel.size() < 3 ||
                                        rel.compare(rel.size() - 3, 3, ".py") != 0) {
                                        rel += ".py";
                                    }
                                    f.outPath = Join(srcDir, rel);
                                    if (writtenPaths.insert(f.outPath).second &&
                                        MakeParentDirs(f.outPath) &&
                                        WriteText(f.outPath, text)) {
                                        r.src += 1;
                                        r.found.push_back(f);
                                        log("pydump: py @" + HexLower(srcAt) +
                                            " " + std::to_string(text.size()) +
                                            "B conf=" + std::to_string(f.confidence));
                                    } else {
                                        r.failed += 1;
                                    }
                                }
                            }
                        }
                    }
                }
            }

            pos += take;
        }
    }

    r.scanned = scanned;
    r.indexJson = BuildIndex(opt, r);
    WriteText(Join(root, "index.json"), r.indexJson);
    log("pydump: " + std::to_string(r.pyc) + " pyc, " + std::to_string(r.src) +
        " source, " + std::to_string(r.failed) + " failed, " +
        std::to_string(scanned / (1024 * 1024)) + " MB scanned");
    return r;
}

}  // namespace pydump