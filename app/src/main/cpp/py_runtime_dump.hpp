// py_runtime_dump.hpp — passive in-process Python dumper.
//
// The target game statically links CPython into libGame.so and exports no
// Py* symbols, so there is nothing to resolve and nothing to call. This module
// never runs Python and never hooks anything: it only reads the process's own
// memory through /proc/self/mem and carves out Python artefacts that are
// already resident:
//
//   * .pyc images  — located by their 4-byte magic, then sized *exactly* by
//                    walking the marshalled code object (so the carved file is
//                    a valid, loadable .pyc rather than a fixed-size guess).
//   * .py sources  — located by strong line-start signatures (shebang, coding
//                    cookie, `if __name__`, top-level def/class/import) and
//                    bounded by blank-line / indentation heuristics.
//
// Everything is read-only and bounded: no writes into the target, no code
// execution, hard caps on bytes scanned and files emitted.

#pragma once

#include <cstdint>
#include <functional>
#include <string>
#include <vector>

namespace pydump {

struct Options {
    std::string outDir;    // root; a "runtime/" subdir is created inside
    std::string gamePkg;   // used for log lines and the index only
    std::string pythonVersion;  // informational, e.g. "3.11.2"

    uint64_t maxTotalBytes = 1536ull * 1024 * 1024;  // stop after this much
    uint64_t maxRegionBytes = 256ull * 1024 * 1024;  // per mapping
    uint64_t maxPycBytes = 8ull * 1024 * 1024;       // sanity bound per carve
    uint64_t maxSrcBytes = 512ull * 1024 * 1024;     // sanity bound per carve

    int maxPyc = 4000;
    int maxSrc = 2000;

    bool scanPyc = true;
    bool scanSource = true;

    // Optional sink; sdk_dumper passes its Logf so progress lands in dump.log.
    std::function<void(const std::string&)> log;
};

struct Found {
    std::string kind;      // "pyc" | "py"
    std::string module;    // code object name (pyc) or module guess (py)
    std::string filename;  // marshalled co_filename (pyc) or source path guess
    uintptr_t address = 0;
    uint64_t size = 0;
    std::string outPath;
    int confidence = 0;    // 0..100
};

struct Result {
    int regions = 0;
    uint64_t scanned = 0;
    int pyc = 0;
    int src = 0;
    int failed = 0;
    std::string indexJson;
    std::vector<Found> found;
};

// Carves everything it can find. Never throws; failures are counted.
Result Run(const Options& opt);

}  // namespace pydump