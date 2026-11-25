#pragma once
// sync_with_py.h
// C++ → Python: kirim config via UDP

#include <string>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <chrono>
#include <mutex>
#include <unordered_map>

#define UDP_SEND_PORT 50000  // C++ → Python
#define UDP_RECV_PORT 50001  // Python → C++ (entity)
#define UDP_SKIN_PORT 50002  // Python → C++ (skin)
#define UDP_HERO_PORT 50003  // Python → C++ (hero)

static int        _swp_sock     = -1;
static std::mutex _swp_mutex;
static std::chrono::steady_clock::time_point _swp_last_sync;

static bool _swp_ensure_sock() {
    if (_swp_sock >= 0) return true;
    _swp_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (_swp_sock < 0) return false;
    int sndbuf = 2 * 1024 * 1024;
    setsockopt(_swp_sock, SOL_SOCKET, SO_SNDBUF, &sndbuf, sizeof(sndbuf));
    return true;
}

static void _swp_send(const std::string& data) {
    std::lock_guard<std::mutex> lk(_swp_mutex);
    if (!_swp_ensure_sock()) return;
    struct sockaddr_in addr{};
    addr.sin_family      = AF_INET;
    addr.sin_port        = htons(UDP_SEND_PORT);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    sendto(_swp_sock, data.c_str(), data.size(), 0,
           (struct sockaddr*)&addr, sizeof(addr));
}

static inline bool _swp_ratelimit(int ms) {
    auto now = std::chrono::steady_clock::now();
    auto el  = std::chrono::duration_cast<std::chrono::milliseconds>(
                   now - _swp_last_sync).count();
    if (el < ms) return false;
    _swp_last_sync = now;
    return true;
}

static std::string _swp_build_json() {
    extern std::unordered_map<std::string, bool>  sync_bool;
    extern std::unordered_map<std::string, float> sync_float;
    extern std::unordered_map<std::string, int>   sync_int;

    std::string d = "{";
    bool first = true;
    char buf[64];
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

static inline void sync_with_py() {
    if (!_swp_ratelimit(100)) return;
    _swp_send(_swp_build_json());
}

static inline void sync_with_py_force() {
    _swp_last_sync = std::chrono::steady_clock::now() -
                     std::chrono::milliseconds(101);
    sync_with_py();
}

static inline void sync_with_py_immediate() {
    _swp_send(_swp_build_json());
}

static inline void sync_with_py_bool(const std::string& key, bool val) {
    if (!_swp_ratelimit(100)) return;
    _swp_send("{\"" + key + "\":" + (val ? "true" : "false") + "}\n");
}

static inline void sync_with_py_int(const std::string& key, int val) {
    if (!_swp_ratelimit(100)) return;
    char buf[128];
    snprintf(buf, sizeof(buf), "{\"%s\":%d}\n", key.c_str(), val);
    _swp_send(buf);
}

static inline void sync_with_py_float(const std::string& key, float val) {
    if (!_swp_ratelimit(100)) return;
    char buf[128];
    snprintf(buf, sizeof(buf), "{\"%s\":%g}\n", key.c_str(), val);
    _swp_send(buf);
}

static inline void sync_with_py_string(const std::string& key, const std::string& val) {
    if (!_swp_ratelimit(100)) return;
    _swp_send("{\"" + key + "\":\"" + val + "\"}\n");
}

static inline void close_sync() {
    std::lock_guard<std::mutex> lk(_swp_mutex);
    if (_swp_sock >= 0) { close(_swp_sock); _swp_sock = -1; }
}
