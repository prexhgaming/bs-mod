#pragma once

#include <string>
#include <vector>
#include <deque>
#include <thread>
#include <mutex>
#include <atomic>
#include <algorithm>
#include <chrono>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include "nlohmann/json.hpp"
#include "entity.h"

using json = nlohmann::json;
using namespace std::chrono;

// ============================================================
// FIFO PATH - satu FIFO dari srcpy
// ============================================================
const char* ENTITY_FIFO_PATH = "/data/data/" PACKAGE_NAME "/imgui/esync";

static constexpr size_t MAX_ENTITY_BUFFER_SIZE = 150 * 1024;
static constexpr size_t MAX_ENTITY_COUNT       = 80;
static constexpr size_t MAX_PENDING_MESSAGES   = 100;
static constexpr size_t MAX_BATCH_SIZE         = 10;
static constexpr auto   READER_SLEEP_TIME      = microseconds(250);
static constexpr auto   PROCESSOR_SLEEP_TIME   = microseconds(500);

// ============================================================
// GLOBALS
// ============================================================
static std::vector<Entity>      direct_entities;
static std::mutex               direct_mutex;
static std::atomic<bool>        has_new_entities{false};

static std::vector<VehicleInfo> global_vehicles;
static std::mutex               vehicle_mutex;

static std::vector<ItemInfo>    global_items;
static std::mutex               item_mutex;

static std::vector<AirdropInfo> global_airdrops;
static std::mutex               airdrop_mutex;

static std::vector<WeaponInfo>  global_weapons;
static std::mutex               weapon_mutex;

static std::atomic<bool>        entity_thread_running{false};
static std::atomic<float>       max_esp_distance{500.0f};
static std::atomic<int>         max_entities_to_process{MAX_ENTITY_COUNT};

static std::deque<std::string>  message_queue;
static std::mutex               queue_mutex;

static std::vector<float>       local_player_pos = {0.f, 0.f, 0.f};
static std::mutex               local_player_mutex;

// ============================================================
// FIFO HELPERS
// ============================================================
static bool ensure_entity_fifo() {
    struct stat st;
    if (stat(ENTITY_FIFO_PATH, &st) == -1) {
        if (errno == ENOENT) return mkfifo(ENTITY_FIFO_PATH, 0666) == 0;
        return false;
    }
    if (!S_ISFIFO(st.st_mode)) {
        unlink(ENTITY_FIFO_PATH);
        return mkfifo(ENTITY_FIFO_PATH, 0666) == 0;
    }
    return true;
}

// ============================================================
// RADAR HELPER
// ============================================================
static void calc_radar(float ex, float ez, float& rx, float& ry, float& rd) {
    std::lock_guard<std::mutex> lk(local_player_mutex);
    float dx = ex - local_player_pos[0];
    float dz = ez - local_player_pos[2];
    rd = sqrtf(dx*dx + dz*dz);
    rx = dx;
    ry = dz;
}

// ============================================================
// PARSE JSON -> structs (sesuai format srcpy)
// ============================================================
static void process_entity_message(const std::string& msg) {
    try {
        auto data = json::parse(msg, nullptr, false);
        if (!data.is_object()) return;

        auto now = steady_clock::now();

        // --- PLAYERS ---
        if (data.contains("players") && data["players"].is_array()) {
            std::vector<Entity> new_entities;
            new_entities.reserve(data["players"].size());

            for (const auto& j : data["players"]) {
                Entity e;
                e.name          = j.value("name", "");
                e.team_id       = j.value("team_id", 0);
                e.is_zombie     = j.value("is_zombie", false);
                e.worldpos      = j.value("worldpos", std::vector<float>{0,0,0});
                e.distance      = j.value("distance", 0.f);
                e.is_on_screen  = j.value("is_on_screen", false);
                e.is_visible    = j.value("is_visible", false);
                e.is_bot        = j.value("is_bot", false);
                e.is_knocked    = j.value("is_knocked", false);
                e.is_alive      = j.value("is_alive", true);
                e.health        = j.value("is_health", 100.f);
                e.armor         = j.value("is_armor", 0.f);
                e.is_ads        = j.value("is_ads", false);
                e.is_shooting   = j.value("is_shooting", false);
                e.is_invincible = j.value("is_invincible", false);
                e.pose_type     = j.value("pose_type", 0);
                e.weapon_id     = j.value("weapon_id", 0);
                e.weapon_name   = j.value("weapon_name", "");
                e.fps_current   = j.contains("fps_info") ? j["fps_info"].value("current", 60) : 60;
                e.bones         = j.value("bones", std::map<std::string, std::vector<float>>());
                e.last_update   = now;

                if (e.worldpos.size() == 3)
                    calc_radar(e.worldpos[0], e.worldpos[2], e.radar_x, e.radar_y, e.radar_distance);

                new_entities.push_back(std::move(e));
            }

            std::sort(new_entities.begin(), new_entities.end(),
                [](const Entity& a, const Entity& b){ return a.distance < b.distance; });

            if (new_entities.size() > static_cast<size_t>(max_entities_to_process.load()))
                new_entities.resize(max_entities_to_process.load());

            std::lock_guard<std::mutex> lk(direct_mutex);
            direct_entities = std::move(new_entities);
            has_new_entities.store(true);
        }

        // --- VEHICLES ---
        if (data.contains("vehicles") && data["vehicles"].is_array()) {
            std::vector<VehicleInfo> vlist;
            for (const auto& j : data["vehicles"]) {
                VehicleInfo v;
                v.name         = j.value("name", "Vehicle");
                v.worldpos     = j.value("worldpos", std::vector<float>{0,0,0});
                v.screenpos    = j.value("screenpos", std::vector<float>{0,0});
                v.distance     = j.value("distance", 0.f);
                v.is_alive     = j.value("is_alive", true);
                v.hp           = j.value("hp", 0.f);
                v.max_health   = j.value("max_health", 100.f);
                v.driver_id    = j.value("driver_id", 0);
                v.is_helicopter= j.value("is_helicopter", false);
                v.vehicle_type = j.value("vehicle_type", 0);
                v.cur_speed    = j.value("cur_speed", 0.f);
                v.last_update  = now;
                if (v.worldpos.size() == 3)
                    calc_radar(v.worldpos[0], v.worldpos[2], v.radar_x, v.radar_y, v.radar_distance);
                vlist.push_back(std::move(v));
            }
            std::lock_guard<std::mutex> lk(vehicle_mutex);
            global_vehicles = std::move(vlist);
        }

        // --- ITEMS ---
        if (data.contains("items") && data["items"].is_array()) {
            std::vector<ItemInfo> ilist;
            for (const auto& j : data["items"]) {
                ItemInfo it;
                it.name      = j.value("name", "Item");
                it.type      = j.value("type", "item");
                it.worldpos  = j.value("worldpos", std::vector<float>{0,0,0});
                it.screenpos = j.value("screenpos", std::vector<float>{0,0});
                it.distance  = j.value("distance", 0.f);
                it.count     = j.value("count", 1);
                ilist.push_back(std::move(it));
            }
            std::lock_guard<std::mutex> lk(item_mutex);
            global_items = std::move(ilist);
        }

        // --- AIRDROPS ---
        if (data.contains("airdrops") && data["airdrops"].is_array()) {
            std::vector<AirdropInfo> alist;
            for (const auto& j : data["airdrops"]) {
                AirdropInfo a;
                a.name      = j.value("name", "AirDrop");
                a.type      = j.value("type", "airdrop");
                a.worldpos  = j.value("worldpos", std::vector<float>{0,0,0});
                a.screenpos = j.value("screenpos", std::vector<float>{0,0});
                a.distance  = j.value("distance", 0.f);
                alist.push_back(std::move(a));
            }
            std::lock_guard<std::mutex> lk(airdrop_mutex);
            global_airdrops = std::move(alist);
        }

        // --- WEAPONS ---
        if (data.contains("weapons") && data["weapons"].is_array()) {
            std::vector<WeaponInfo> wlist;
            for (const auto& j : data["weapons"]) {
                WeaponInfo w;
                w.name     = j.value("name", "");
                w.id       = j.value("id", 0);
                w.position = j.value("position", std::vector<float>{0,0,0});
                w.distance = j.value("distance", 0.f);
                w.type     = j.value("type", "Other");
                wlist.push_back(std::move(w));
            }
            std::lock_guard<std::mutex> lk(weapon_mutex);
            global_weapons = std::move(wlist);
        }

    } catch (...) {}
}

// ============================================================
// PROCESSOR THREAD
// ============================================================
static void message_processor_thread_func() {
    while (entity_thread_running.load()) {
        std::vector<std::string> batch;
        {
            std::unique_lock<std::mutex> lk(queue_mutex, std::try_to_lock);
            if (lk.owns_lock() && !message_queue.empty()) {
                size_t n = std::min(message_queue.size(), MAX_BATCH_SIZE);
                batch.reserve(n);
                for (size_t i = 0; i < n; ++i) {
                    batch.push_back(std::move(message_queue.front()));
                    message_queue.pop_front();
                }
            }
        }
        for (const auto& m : batch) process_entity_message(m);
        std::this_thread::sleep_for(PROCESSOR_SLEEP_TIME);
    }
}

// ============================================================
// READER THREAD
// ============================================================
static void entity_reader_thread_func() {
    if (!ensure_entity_fifo()) return;

    std::thread(message_processor_thread_func).detach();

    int fd = -1;
    std::vector<char> buf(MAX_ENTITY_BUFFER_SIZE);
    size_t offset = 0;

    while (entity_thread_running.load()) {
        if (fd < 0) {
            fd = open(ENTITY_FIFO_PATH, O_RDONLY | O_NONBLOCK);
            if (fd < 0) { std::this_thread::sleep_for(READER_SLEEP_TIME); continue; }
            offset = 0;
        }

        struct pollfd pfd = {fd, POLLIN, 0};
        if (poll(&pfd, 1, 25) < 0) { close(fd); fd = -1; continue; }

        if (pfd.revents & POLLIN) {
            ssize_t n = read(fd, buf.data() + offset, buf.size() - offset - 1);
            if (n <= 0) {
                if (n < 0 && errno != EAGAIN) { close(fd); fd = -1; }
                continue;
            }
            offset += n;
            buf[offset] = '\0';

            char* start = buf.data();
            char* end;
            while ((end = strchr(start, '\n')) != nullptr) {
                *end = '\0';
                {
                    std::unique_lock<std::mutex> lk(queue_mutex, std::try_to_lock);
                    if (lk.owns_lock() && message_queue.size() < MAX_PENDING_MESSAGES)
                        message_queue.emplace_back(start);
                }
                start = end + 1;
            }
            size_t rem = offset - (start - buf.data());
            memmove(buf.data(), start, rem);
            offset = rem;

        } else if (pfd.revents & (POLLERR | POLLHUP | POLLNVAL)) {
            close(fd); fd = -1;
        }
    }
    if (fd >= 0) close(fd);
}

// ============================================================
// START / STOP
// ============================================================
static void start_entity_reader() {
    if (entity_thread_running.exchange(true)) return;
    std::thread(entity_reader_thread_func).detach();
}
static void stop_entity_reader() {
    entity_thread_running.store(false);
}

// ============================================================
// GETTERS
// ============================================================
static std::vector<Entity> get_entities() {
    std::lock_guard<std::mutex> lk(direct_mutex);
    const float maxd = max_esp_distance.load();
    std::vector<Entity> result;
    result.reserve(direct_entities.size());
    for (const auto& e : direct_entities)
        if (e.IsValid() && e.distance <= maxd) result.push_back(e);
    return result;
}

static std::vector<VehicleInfo> get_vehicles() {
    std::lock_guard<std::mutex> lk(vehicle_mutex);
    std::vector<VehicleInfo> result;
    for (const auto& v : global_vehicles)
        if (v.IsValid() && v.IsDataFresh()) result.push_back(v);
    return result;
}

static std::vector<ItemInfo> get_items() {
    std::lock_guard<std::mutex> lk(item_mutex);
    return global_items;
}

static std::vector<AirdropInfo> get_airdrops() {
    std::lock_guard<std::mutex> lk(airdrop_mutex);
    return global_airdrops;
}

static std::vector<WeaponInfo> get_weapons() {
    std::lock_guard<std::mutex> lk(weapon_mutex);
    return global_weapons;
}

static void set_max_esp_distance(float d) {
    max_esp_distance.store(std::clamp(d, 0.f, 500.f));
}
static float get_max_esp_distance() { return max_esp_distance.load(); }
static void set_max_entities(int n) {
    max_entities_to_process.store(std::clamp(n, 1, (int)MAX_ENTITY_COUNT));
}
