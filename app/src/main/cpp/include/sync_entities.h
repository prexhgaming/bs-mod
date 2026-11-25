#pragma once

#include <string>
#include <vector>
#include <thread>
#include <atomic>
#include <unordered_map>
#include <chrono>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <poll.h>
#include <errno.h>
#include <string.h>
#include <algorithm>
#include <cmath>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "nlohmann/json.hpp"
#include "entity.h"

using json = nlohmann::json;
using namespace std::chrono;

extern const char* PACKAGE_NAME;
extern int Width;
extern int Height;

// ============================================================
// 🔥 UDP CONFIGURATION
// ============================================================
#define UDP_RECV_PORT 50001   // Python -> C++ (ESP data)

static int g_udp_recv_sock = -1;
static struct sockaddr_in g_udp_recv_addr;
static std::atomic<bool> g_udp_recv_initialized{false};

static std::vector<Entity>   current_entities;
static std::mutex            entities_mutex;
static std::atomic<bool>     entities_updated{false};
static std::atomic<uint64_t> update_counter{0};
static std::atomic<bool>     entity_thread_running{false};
static std::atomic<int64_t>  g_last_data_ms{0};
static constexpr int64_t     STALE_TIMEOUT_MS = 800;

static std::vector<float>    local_player_pos = {0.f, 0.f, 0.f};
static std::mutex            local_player_mutex;

static float g_cam_yaw  = 0.f;
static float g_screen_w = 1080.f;
static float g_screen_h = 1920.f;

// ============ INIT UDP RECEIVER ============
static bool init_udp_receiver() {
    if (g_udp_recv_initialized.load() && g_udp_recv_sock >= 0) {
        return true;
    }
    
    if (g_udp_recv_sock >= 0) {
        close(g_udp_recv_sock);
        g_udp_recv_sock = -1;
    }
    
    g_udp_recv_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (g_udp_recv_sock < 0) {
        return false;
    }
    
    int bufsize = 2 * 1024 * 1024;
    setsockopt(g_udp_recv_sock, SOL_SOCKET, SO_RCVBUF, &bufsize, sizeof(bufsize));
    
    g_udp_recv_addr.sin_family = AF_INET;
    g_udp_recv_addr.sin_port = htons(UDP_RECV_PORT);
    g_udp_recv_addr.sin_addr.s_addr = INADDR_ANY;
    
    // 🔥 FIX: pake ::bind
    if (::bind(g_udp_recv_sock, (struct sockaddr*)&g_udp_recv_addr, sizeof(g_udp_recv_addr)) < 0) {
        close(g_udp_recv_sock);
        g_udp_recv_sock = -1;
        return false;
    }
    
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 1000;
    setsockopt(g_udp_recv_sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
    
    g_udp_recv_initialized.store(true);
    return true;
}

static void calc_radar(float ex, float ez, float& rx, float& ry, float& rd) {
    std::lock_guard<std::mutex> lk(local_player_mutex);
    float dx = ex - local_player_pos[0];
    float dz = ez - local_player_pos[2];
    rd = sqrtf(dx * dx + dz * dz);
    rx = dx; ry = dz;
}

static Entity parse_player_entity(const json& j,
    const std::string& _world_name, const std::string& _real_world_name,
    bool _is_moba, bool _is_replaying, int _total_entities,
    const steady_clock::time_point& now)
{
    Entity e;
    e.name             = j.value("name",             "");
    e.team_id          = j.value("team_id",          0);
    e.hero_id          = j.value("hero_id",          0);
    e.is_zombie        = j.value("is_zombie",        false);
    e.is_bot           = j.value("is_bot",           false);
    e.is_teammate      = j.value("is_teammate",      false);
    e.worldpos         = j.value("worldpos",         std::vector<float>{0,0,0});
    e.distance         = j.value("distance",         0.f);
    e.is_on_screen     = j.value("is_on_screen",     false);
    e.is_visible       = j.value("is_visible",       false);
    e.bones            = j.value("bones",            std::unordered_map<std::string, std::vector<float>>());
    
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
    
    e.health           = j.value("is_health",        100.f);
    e.max_health       = j.value("cur_maxhp",        100.f);
    e.armor            = j.value("is_armor",         0.f);
    e.base_armor       = j.value("base_armor",       0.f);
    e.extra_armor      = j.value("extra_armor",      0.f);
    e.is_alive         = j.value("is_alive",         true);
    e.is_knocked       = j.value("is_knocked",       false);
    e.is_dead          = j.value("is_dead",          false);
    e.is_invincible    = j.value("is_invincible",    false);
    e.pose_type        = j.value("pose_type",        0);
    e.is_shield_active = j.value("is_shield_active", false);
    e.shield_hp        = j.value("shield_hp",        0.f);
    e.shield_maxhp     = j.value("shield_maxhp",     100.f);
    e.weapon_id        = j.value("weapon_id",        0);
    e.weapon_name      = j.value("weapon_name",      "");
    e.weapon_gun_type  = j.value("weapon_gun_type",  0);
    e.optic_type       = j.value("optic_type",       0);
    e.is_reloading     = j.value("is_reloading",     false);
    if (j.contains("weapon_list") && j["weapon_list"].is_array())
        e.weapon_list = j["weapon_list"].get<std::vector<int>>();
    e.is_ads           = j.value("is_ads",           false);
    e.is_real_ads      = j.value("is_real_ads",      false);
    e.is_shooting      = j.value("is_shooting",      false);
    e.is_sprint        = j.value("is_sprint",        false);
    e.is_super_sprint  = j.value("is_super_sprint",  false);
    e.is_fps_mode      = j.value("is_fps_mode",      false);
    e.is_on_sky        = j.value("is_on_sky",        false);
    e.is_in_swim       = j.value("is_in_swim",       false);
    e.is_veh_driver    = j.value("is_veh_driver",    false);
    e.is_veh_passenger = j.value("is_veh_passenger", false);
    e.is_reviving      = j.value("is_reviving",      false);
    e.is_being_revived = j.value("is_being_revived", false);
    e.yaw              = j.value("yaw",              0.f);
    e.velocity_x       = j.value("velocity_x",       0.f);
    e.velocity_y       = j.value("velocity_y",       0.f);
    e.velocity_z       = j.value("velocity_z",       0.f);
    e.in_house         = j.value("in_house",         "");
    if (j.contains("buffs") && j["buffs"].is_array())
        e.buffs = j["buffs"].get<std::vector<int>>();
    if (j.contains("bone_depths") && j["bone_depths"].is_array()) {
        auto depths = j["bone_depths"].get<std::vector<float>>();
        for (size_t i = 0; i < depths.size() && i < BONE_COUNT; i++)
            e.bone_depth[i] = depths[i];
    }
    e.world_name      = _world_name;
    e.real_world_name = _real_world_name;
    e.is_moba_map     = _is_moba;
    e.is_replaying    = _is_replaying;
    e.total_entities  = _total_entities;
    e.last_update     = now;
    if (e.worldpos.size() == 3)
        calc_radar(e.worldpos[0], e.worldpos[2], e.radar_x, e.radar_y, e.radar_distance);
    e.RebuildBoneCache();
    return e;
}

static bool read_and_process_udp() {
    if (!init_udp_receiver() || g_udp_recv_sock < 0) {
        return false;
    }
    
    static char buf[65536];
    struct sockaddr_in from;
    socklen_t fromlen = sizeof(from);
    
    int n = recvfrom(g_udp_recv_sock, buf, sizeof(buf) - 1, 0,
                     (struct sockaddr*)&from, &fromlen);
    
    if (n <= 0) {
        return false;
    }
    buf[n] = '\0';
    
    try {
        auto data = json::parse(buf, nullptr, false);
        if (!data.is_object()) return false;
        
        auto        now              = steady_clock::now();
        std::string _world_name      = data.value("world_name",      "");
        std::string _real_world_name = data.value("real_world_name", "");
        bool        _is_moba         = data.value("is_moba_map",     false);
        bool        _is_replaying    = data.value("is_replaying",    false);
        int         _total_entities  = data.value("total_entities",  0);
        
        g_cam_yaw  = data.value("cam_yaw",  0.f);
        g_screen_w = data.value("screen_w", 1080.f);
        g_screen_h = data.value("screen_h", 1920.f);
        
        if (!data.contains("players") || !data["players"].is_array()) {
            std::lock_guard<std::mutex> lk(entities_mutex);
            current_entities.clear();
            entities_updated.store(true, std::memory_order_release);
            update_counter.fetch_add(1, std::memory_order_release);
            return true;
        }
        
        const auto& players_array = data["players"];
        std::vector<Entity> new_entities;
        new_entities.reserve(players_array.size());
        
        for (const auto& j : players_array) {
            if (!j.value("is_alive", true) || j.value("is_dead", false)) continue;
            new_entities.push_back(
                parse_player_entity(j, _world_name, _real_world_name,
                                    _is_moba, _is_replaying, _total_entities, now));
        }
        
        std::sort(new_entities.begin(), new_entities.end(),
            [](const Entity& a, const Entity& b){ return a.distance < b.distance; });
        
        {
            std::lock_guard<std::mutex> lk(entities_mutex);
            current_entities = std::move(new_entities);
            entities_updated.store(true, std::memory_order_release);
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
        usleep(1000);  // 1ms sleep agar tidak 100% CPU
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

static void stop_entity_reader() {
    entity_thread_running.store(false, std::memory_order_release);
    if (g_udp_recv_sock >= 0) {
        close(g_udp_recv_sock);
        g_udp_recv_sock = -1;
    }
    g_udp_recv_initialized.store(false);
}

static std::vector<Entity> _frame_cache;
static uint64_t            _frame_cache_counter = UINT64_MAX;

static const std::vector<Entity>& get_frame_entities() {
    int64_t nowMs = duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
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

static float get_cam_yaw()  { return g_cam_yaw;  }
static float get_screen_w() { return g_screen_w; }
static float get_screen_h() { return g_screen_h; }
