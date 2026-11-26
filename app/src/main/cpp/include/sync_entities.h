#pragma once

// Python -> C++ entity transport. The injected script sends one JSON datagram
// per tick to 127.0.0.1:UDP_RECV_PORT; this reader turns it into Entity records
// for the ESP. Nothing here goes back to the script — the UI talks to it
// through data.json (see sync_with_py.h).

#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>
#include <unordered_map>
#include <chrono>
#include <cmath>
#include <algorithm>
#include <unistd.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "nlohmann/json.hpp"
#include "entity.h"

using json = nlohmann::json;
using namespace std::chrono;

extern int Width;
extern int Height;

#define UDP_RECV_PORT 50001   // Python -> C++ (entity data)

static int g_udp_recv_sock = -1;
static struct sockaddr_in g_udp_recv_addr;
static std::atomic<bool> g_udp_recv_initialized{false};

static std::vector<Entity>   current_entities;
static std::mutex            entities_mutex;
static std::atomic<uint64_t> update_counter{0};
static std::atomic<bool>     entity_thread_running{false};
static std::atomic<int64_t>  g_last_data_ms{0};
static constexpr int64_t     STALE_TIMEOUT_MS = 800;

// Resolution the script projected with, so bones can be rescaled to the
// overlay surface. 0 means "unknown" - the rescale is then skipped (1:1).
static float g_screen_w = 0.f;
static float g_screen_h = 0.f;

static bool init_udp_receiver() {
    if (g_udp_recv_initialized.load() && g_udp_recv_sock >= 0) return true;

    if (g_udp_recv_sock >= 0) {
        close(g_udp_recv_sock);
        g_udp_recv_sock = -1;
    }

    g_udp_recv_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (g_udp_recv_sock < 0) return false;

    int bufsize = 2 * 1024 * 1024;
    setsockopt(g_udp_recv_sock, SOL_SOCKET, SO_RCVBUF, &bufsize, sizeof(bufsize));

    g_udp_recv_addr.sin_family      = AF_INET;
    g_udp_recv_addr.sin_port        = htons(UDP_RECV_PORT);
    g_udp_recv_addr.sin_addr.s_addr = INADDR_ANY;

    if (::bind(g_udp_recv_sock, (struct sockaddr*)&g_udp_recv_addr,
               sizeof(g_udp_recv_addr)) < 0) {
        close(g_udp_recv_sock);
        g_udp_recv_sock = -1;
        return false;
    }

    struct timeval tv;
    tv.tv_sec  = 0;
    tv.tv_usec = 1000;
    setsockopt(g_udp_recv_sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

    g_udp_recv_initialized.store(true);
    return true;
}

// Mirrors the entry dict built in srcpy.cpp. Keys the script omits (health,
// armor and weapon are only sent when the matching ESP toggle is on) fall back
// to the defaults below.
static Entity parse_player_entity(const json& j, const steady_clock::time_point& now) {
    Entity e;
    e.name          = j.value("name",          "");
    e.is_zombie     = j.value("is_zombie",     false);
    e.is_bot        = j.value("is_bot",        false);
    e.is_teammate   = j.value("is_teammate",   false);
    e.distance      = j.value("distance",      0.f);
    e.is_on_screen  = j.value("is_on_screen",  false);
    e.is_visible    = j.value("is_visible",    false);
    e.bones         = j.value("bones",         std::unordered_map<std::string, std::vector<float>>());
    e.health        = j.value("is_health",     100.f);
    e.max_health    = j.value("cur_maxhp",     100.f);
    e.armor         = j.value("is_armor",      0.f);
    e.is_alive      = j.value("is_alive",      true);
    e.is_knocked    = j.value("is_knocked",    false);
    e.is_dead       = j.value("is_dead",       false);
    e.weapon_name   = j.value("weapon_name",   "");

    // loot ESP
    e.is_item       = j.value("is_item",       false);
    e.item_label    = j.value("item_label",    "");
    e.item_count    = j.value("item_count",    1);

    if (g_screen_w > 1.f && g_screen_h > 1.f && Width > 0 && Height > 0) {
        float sx = (float)Width  / g_screen_w;
        float sy = (float)Height / g_screen_h;
        if (std::fabs(sx - 1.f) > 0.001f || std::fabs(sy - 1.f) > 0.001f) {
            for (auto& kv : e.bones) {
                if (kv.second.size() >= 2) {
                    kv.second[0] *= sx;
                    kv.second[1] *= sy;
                }
            }
        }
    }

    e.RebuildBoneCache();
    return e;
}

static bool read_and_process_udp() {
    if (!init_udp_receiver() || g_udp_recv_sock < 0) return false;

    static char buf[65536];
    struct sockaddr_in from;
    socklen_t fromlen = sizeof(from);

    int n = recvfrom(g_udp_recv_sock, buf, sizeof(buf) - 1, 0,
                     (struct sockaddr*)&from, &fromlen);
    if (n <= 0) return false;
    buf[n] = '\0';

    try {
        auto data = json::parse(buf, nullptr, false);
        if (!data.is_object()) return false;

        auto now = steady_clock::now();

        // 0 = "the script did not tell us", which disables the rescale below.
        // A wrong guess here is much worse than no rescale: the old 1080x1920
        // default silently stretched every bone by Height/1920 on a 2400px
        // screen, putting the ESP well below the players.
        g_screen_w = data.value("screen_w", 0.f);
        g_screen_h = data.value("screen_h", 0.f);

        if (!data.contains("players") || !data["players"].is_array()) {
            std::lock_guard<std::mutex> lk(entities_mutex);
            current_entities.clear();
            update_counter.fetch_add(1, std::memory_order_release);
            return true;
        }

        const auto& players_array = data["players"];
        std::vector<Entity> new_entities;
        new_entities.reserve(players_array.size());

        for (const auto& j : players_array) {
            if (!j.value("is_alive", true) || j.value("is_dead", false)) continue;
            new_entities.push_back(parse_player_entity(j, now));
        }

        std::sort(new_entities.begin(), new_entities.end(),
            [](const Entity& a, const Entity& b){ return a.distance < b.distance; });

        {
            std::lock_guard<std::mutex> lk(entities_mutex);
            current_entities = std::move(new_entities);
            update_counter.fetch_add(1, std::memory_order_release);
        }
        g_last_data_ms.store(
            duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count(),
            std::memory_order_release);
        return true;
    } catch (...) {}

    return false;
}

static void entity_reader_thread_func() {
    init_udp_receiver();

    while (entity_thread_running.load(std::memory_order_relaxed)) {
        if (!g_udp_recv_initialized.load() || g_udp_recv_sock < 0) {
            usleep(10000);
            continue;
        }
        read_and_process_udp();
        usleep(1000);
    }

    if (g_udp_recv_sock >= 0) {
        close(g_udp_recv_sock);
        g_udp_recv_sock = -1;
    }
    g_udp_recv_initialized.store(false);
}

static void start_entity_reader() {
    if (entity_thread_running.exchange(true)) return;
    init_udp_receiver();
    std::thread(entity_reader_thread_func).detach();
}

// Per-frame snapshot. Rebuilt only when a new datagram arrived, and emptied
// once the script goes quiet so a frozen game does not leave stale boxes up.
static std::vector<Entity> _frame_cache;
static uint64_t            _frame_cache_counter = UINT64_MAX;

static const std::vector<Entity>& get_frame_entities() {
    int64_t nowMs  = duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
    int64_t lastMs = g_last_data_ms.load(std::memory_order_acquire);

    if (lastMs != 0 && (nowMs - lastMs) > STALE_TIMEOUT_MS) {
        if (!_frame_cache.empty()) {
            _frame_cache.clear();
            std::lock_guard<std::mutex> lk(entities_mutex);
            current_entities.clear();
        }
        return _frame_cache;
    }

    uint64_t cur = update_counter.load(std::memory_order_acquire);
    if (cur != _frame_cache_counter) {
        std::lock_guard<std::mutex> lk(entities_mutex);
        _frame_cache.clear();
        _frame_cache.reserve(current_entities.size());
        for (const auto& e : current_entities)
            if (e.IsValid()) _frame_cache.push_back(e);
        _frame_cache_counter = cur;
    }
    return _frame_cache;
}
