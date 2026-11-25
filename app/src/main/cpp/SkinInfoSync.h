#pragma once
// SkinInfoSync.h — Terima skin & hero info dari Python via UDP
// Port 50002 = skin info, Port 50003 = hero info

#include <string>
#include <vector>
#include <unordered_map>
#include <mutex>
#include <thread>
#include <atomic>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <algorithm>
#include <sstream>

namespace SkinInfoSync {

static std::mutex g_mutex;
static std::unordered_map<int, std::string>              g_weaponNames;
static std::unordered_map<int, std::vector<std::string>> g_skinNames;
static std::atomic<bool> g_dataUpdated{false};

static std::mutex g_heroMutex;
static std::unordered_map<int, std::string>              g_heroNames;
static std::unordered_map<int, std::vector<std::string>> g_heroSkinNames;
static std::unordered_map<int, std::vector<int>>         g_heroSkinLevels;
static std::atomic<bool> g_heroDataUpdated{false};

static std::atomic<bool> g_started{false};

// ─── Line parsers ─────────────────────────────────────────────────────────────
static void _parseLine(const std::string& line) {
    if (line.empty()) return;
    if (line[0] == 'W') {
        size_t p1 = line.find('|'), p2 = line.find('|', p1+1);
        if (p1==std::string::npos||p2==std::string::npos) return;
        int id = atoi(line.substr(p1+1, p2-p1-1).c_str());
        std::lock_guard<std::mutex> lk(g_mutex);
        g_weaponNames[id] = line.substr(p2+1);
        if (g_skinNames.find(id)==g_skinNames.end()) g_skinNames[id]={};
    } else if (line[0] == 'S') {
        size_t p1=line.find('|'), p2=line.find('|',p1+1), p3=line.find('|',p2+1);
        if (p1==std::string::npos||p2==std::string::npos||p3==std::string::npos) return;
        int id  = atoi(line.substr(p1+1, p2-p1-1).c_str());
        int idx = atoi(line.substr(p2+1, p3-p2-1).c_str());
        std::string name = line.substr(p3+1);
        std::lock_guard<std::mutex> lk(g_mutex);
        auto& vec = g_skinNames[id];
        if ((int)vec.size() < idx) vec.resize(idx);
        vec[idx-1] = name;
    } else if (line == "END") {
        g_dataUpdated.store(true);
    }
}

static void _parseHeroLine(const std::string& line) {
    if (line.empty()) return;
    if (line[0] == 'H') {
        size_t p1=line.find('|'), p2=line.find('|',p1+1);
        if (p1==std::string::npos||p2==std::string::npos) return;
        int id = atoi(line.substr(p1+1, p2-p1-1).c_str());
        std::lock_guard<std::mutex> lk(g_heroMutex);
        g_heroNames[id] = line.substr(p2+1);
        if (g_heroSkinNames.find(id)==g_heroSkinNames.end())  g_heroSkinNames[id]={};
        if (g_heroSkinLevels.find(id)==g_heroSkinLevels.end()) g_heroSkinLevels[id]={};
    } else if (line[0] == 'K') {
        size_t p1=line.find('|'), p2=line.find('|',p1+1),
               p3=line.find('|',p2+1), p4=line.find('|',p3+1);
        if (p1==std::string::npos||p2==std::string::npos||p3==std::string::npos) return;
        int id  = atoi(line.substr(p1+1, p2-p1-1).c_str());
        int idx = atoi(line.substr(p2+1, p3-p2-1).c_str());
        std::string name = line.substr(p3+1, p4==std::string::npos ? std::string::npos : p4-p3-1);
        int level = p4!=std::string::npos ? atoi(line.substr(p4+1).c_str()) : 0;
        std::lock_guard<std::mutex> lk(g_heroMutex);
        auto& nv = g_heroSkinNames[id];
        if ((int)nv.size()<idx) nv.resize(idx);
        nv[idx-1] = name;
        auto& lv = g_heroSkinLevels[id];
        if ((int)lv.size()<idx) lv.resize(idx);
        lv[idx-1] = level;
    } else if (line == "END") {
        g_heroDataUpdated.store(true);
    }
}

// ─── UDP reader thread ────────────────────────────────────────────────────────
static void _udpReaderThread(int port, bool is_hero) {
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0) return;

    int rcvbuf = 2*1024*1024;
    setsockopt(fd, SOL_SOCKET, SO_RCVBUF, &rcvbuf, sizeof(rcvbuf));

    struct timeval tv{0, 10000};
    setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

    struct sockaddr_in addr{};
    addr.sin_family      = AF_INET;
    addr.sin_port        = htons(port);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    if (::bind(fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        close(fd); return;
    }

    char buf[65536];
    std::string accum;

    while (true) {
        ssize_t n = recv(fd, buf, sizeof(buf)-1, 0);
        if (n <= 0) { usleep(10000); continue; }
        buf[n] = '\0';
        accum += buf;

        size_t pos = 0, nl;
        while ((nl = accum.find('\n', pos)) != std::string::npos) {
            std::string line = accum.substr(pos, nl-pos);
            while (!line.empty() && (line.back()=='\r'||line.back()=='\n'))
                line.pop_back();
            pos = nl+1;
            is_hero ? _parseHeroLine(line) : _parseLine(line);
        }
        accum = accum.substr(pos);
    }
    close(fd);
}

static void _injectHardcore() {
    std::lock_guard<std::mutex> lk(g_mutex);
    g_weaponNames[33] = "DANGER";
    auto& vec = g_skinNames[33];
    if ((int)vec.size() < 13) vec.resize(13);
    vec[11] = "Hardcore PIVOT";
    vec[12] = "Hardcore NEXUS";
}

// ─── Public API ───────────────────────────────────────────────────────────────
static inline void Init() {
    if (g_started.exchange(true)) return;
    _injectHardcore();
    std::thread([]{ _udpReaderThread(50002, false); }).detach();
    std::thread([]{ _udpReaderThread(50003, true);  }).detach();
}

static inline bool GetSkinName(int equipId, int idx1, char* out, size_t sz) {
    std::lock_guard<std::mutex> lk(g_mutex);
    auto it = g_skinNames.find(equipId);
    if (it==g_skinNames.end()||idx1<1||idx1>(int)it->second.size()) return false;
    const std::string& s = it->second[idx1-1];
    if (s.empty()) return false;
    snprintf(out, sz, "%s", s.c_str()); return true;
}

static inline bool GetWeaponName(int equipId, char* out, size_t sz) {
    std::lock_guard<std::mutex> lk(g_mutex);
    auto it = g_weaponNames.find(equipId);
    if (it==g_weaponNames.end()) return false;
    snprintf(out, sz, "%s", it->second.c_str()); return true;
}

static inline int GetSkinCount(int equipId) {
    std::lock_guard<std::mutex> lk(g_mutex);
    auto it = g_skinNames.find(equipId);
    return it==g_skinNames.end() ? 0 : (int)it->second.size();
}

static inline bool GetHeroName(int heroId, char* out, size_t sz) {
    std::lock_guard<std::mutex> lk(g_heroMutex);
    auto it = g_heroNames.find(heroId);
    if (it==g_heroNames.end()) return false;
    snprintf(out, sz, "%s", it->second.c_str()); return true;
}

static inline bool GetHeroSkinName(int heroId, int idx1, char* out, size_t sz) {
    std::lock_guard<std::mutex> lk(g_heroMutex);
    auto it = g_heroSkinNames.find(heroId);
    if (it==g_heroSkinNames.end()||idx1<1||idx1>(int)it->second.size()) return false;
    const std::string& s = it->second[idx1-1];
    if (s.empty()) return false;
    snprintf(out, sz, "%s", s.c_str()); return true;
}

static inline int GetHeroSkinLevel(int heroId, int idx1) {
    std::lock_guard<std::mutex> lk(g_heroMutex);
    auto it = g_heroSkinLevels.find(heroId);
    if (it==g_heroSkinLevels.end()||idx1<1||idx1>(int)it->second.size()) return 0;
    return it->second[idx1-1];
}

static inline int GetHeroSkinCount(int heroId) {
    std::lock_guard<std::mutex> lk(g_heroMutex);
    auto it = g_heroSkinNames.find(heroId);
    return it==g_heroSkinNames.end() ? 0 : (int)it->second.size();
}

static inline std::vector<int> GetAllWeaponIds() {
    std::lock_guard<std::mutex> lk(g_mutex);
    std::vector<int> ids;
    for (auto& kv : g_weaponNames) ids.push_back(kv.first);
    std::sort(ids.begin(), ids.end());
    return ids;
}

static inline std::vector<int> GetAllHeroIds() {
    std::lock_guard<std::mutex> lk(g_heroMutex);
    std::vector<int> ids;
    for (auto& kv : g_heroNames) ids.push_back(kv.first);
    std::sort(ids.begin(), ids.end());
    return ids;
}

static inline bool IsWeaponDataReady() { return g_dataUpdated.load() || !g_weaponNames.empty(); }
static inline bool IsHeroDataReady()   { return g_heroDataUpdated.load() || !g_heroNames.empty(); }

} // namespace SkinInfoSync
