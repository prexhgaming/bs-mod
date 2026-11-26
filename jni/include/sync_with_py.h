#pragma once
// sync_with_py.h
// C++ -> Python: write data.json to disk (no UDP port)
// Python -> C++: entity data via one UDP port (UDP_RECV_PORT)
//
// The UI only mutates the sync_* maps. Nothing is pushed at Python: the
// injected script reads data.json and acts on whatever values it finds, so the
// direction is one-way and the C++ side never has to know whether Python is
// even running.
//
// data.json is deliberately NOT config.json. draw.h's SaveConfig()/LoadConfig()
// own config.json and store a nested {"bool":{..},"float":{..},"int":{..}}
// document there so the user's settings survive a restart. This file is a flat
// {key: value} snapshot of the live maps, rewritten whenever a value changes.
// Sharing one path would let the flat write clobber the nested one and silently
// wipe the saved settings on the next launch.

#include <string>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <chrono>
#include <mutex>
#include <unordered_map>
#include <fstream>

// Python -> C++ entity data uses UDP_RECV_PORT, defined in sync_entities.h.

static std::mutex _swp_mutex;
static std::chrono::steady_clock::time_point _swp_last_sync;
static std::string _swp_last_written;   // last JSON written, to skip no-op writes
static std::string _swp_last_path;      // path that JSON went to

static inline bool _swp_ratelimit(int ms) {
    auto now = std::chrono::steady_clock::now();
    auto el  = std::chrono::duration_cast<std::chrono::milliseconds>(
                   now - _swp_last_sync).count();
    if (el < ms) return false;
    _swp_last_sync = now;
    return true;
}

// Defined later in draw.h (which is the only includer of this header). Using
// it here matters: it mkdir's the directory it returns, and it resolves to the
// game's internal data dir whenever the activity is up, falling back to
// CONFIG_DIR only otherwise. Hardcoding a path instead meant writing into a
// directory nothing ever created, so every flush failed silently and the
// script fell back to its own 1080x720 defaults for fWidth/fHeight - which in
// turn made the entity parser rescale bones by Height/720 and put the ESP
// nowhere near the player.
static inline std::string GetConfigDir();

// Same directory draw.h uses for config.json / imgui.ini, but its own file.
static std::string _swp_data_path() {
    const char* env = getenv("NEXORA_CONFIG_DIR");
    if (env && env[0]) return std::string(env) + "data.json";
    return GetConfigDir() + "data.json";
}

static std::string _swp_build_json() {
    extern std::unordered_map<std::string, bool>  sync_bool;
    extern std::unordered_map<std::string, float> sync_float;
    extern std::unordered_map<std::string, int>   sync_int;

    std::string d = "{";
    bool first = true;
    char buf[96];
    for (const auto& [k, v] : sync_bool) {
        if (!first) d += ',';
        d += '"'; d += k; d += '"'; d += ':';
        d += v ? "true" : "false";
        first = false;
    }
    for (const auto& [k, v] : sync_float) {
        if (!first) d += ',';
        snprintf(buf, sizeof(buf), "\"%s\":%g", k.c_str(), v);
        d += buf; first = false;
    }
    for (const auto& [k, v] : sync_int) {
        if (!first) d += ',';
        snprintf(buf, sizeof(buf), "\"%s\":%d", k.c_str(), v);
        d += buf; first = false;
    }
    d += "}\n";
    return d;
}

// Write the file only when the payload actually changed. The menu calls this
// every frame, and the maps are static most of the time, so this keeps it at
// roughly one write per user interaction instead of 60+ per second.
static void _swp_flush(const std::string& json) {
    std::lock_guard<std::mutex> lk(_swp_mutex);
    std::string path = _swp_data_path();
    // Skip only when both the payload and the destination are unchanged: the
    // resolved directory can move from the CONFIG_DIR fallback to the internal
    // data dir once the activity is up, and the new file must be written then.
    if (json == _swp_last_written && path == _swp_last_path) return;
    std::ofstream f(path, std::ios::trunc);
    if (!f.is_open()) return;
    f << json;
    f.close();
    if (f.good() || !f.bad()) { _swp_last_written = json; _swp_last_path = path; }
}

static inline void sync_with_py() {
    if (!_swp_ratelimit(100)) return;
    _swp_flush(_swp_build_json());
}

static inline void sync_with_py_force() {
    _swp_last_sync = std::chrono::steady_clock::now() -
                     std::chrono::milliseconds(101);
    _swp_flush(_swp_build_json());
}

static inline void sync_with_py_immediate() {
    _swp_flush(_swp_build_json());
}

// Per-key helpers kept for call sites that only touch one value; they merge
// into the full config rather than clobbering it with a one-key object.
static inline void sync_with_py_bool(const std::string& key, bool val) {
    if (!_swp_ratelimit(100)) return;
    extern std::unordered_map<std::string, bool> sync_bool;
    sync_bool[key] = val;
    _swp_flush(_swp_build_json());
}

static inline void sync_with_py_int(const std::string& key, int val) {
    if (!_swp_ratelimit(100)) return;
    extern std::unordered_map<std::string, int> sync_int;
    sync_int[key] = val;
    _swp_flush(_swp_build_json());
}

static inline void sync_with_py_float(const std::string& key, float val) {
    if (!_swp_ratelimit(100)) return;
    extern std::unordered_map<std::string, float> sync_float;
    sync_float[key] = val;
    _swp_flush(_swp_build_json());
}

static inline void sync_with_py_string(const std::string& key, const std::string& val) {
    (void)key; (void)val;  // no string channel: config is bool/float/int only
}

// No socket to close any more; kept so existing teardown calls still compile.
static inline void close_sync() {
    std::lock_guard<std::mutex> lk(_swp_mutex);
    _swp_last_written.clear();
    _swp_last_path.clear();
}
