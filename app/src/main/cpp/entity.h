#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <chrono>
#include <cmath>
#include <cfloat>
#include <algorithm>
#include "imgui/imgui.h"

// ============================================================================
// BONE INDEX & NAMES - FIXED! Match dengan srcpy.cpp
// ============================================================================
enum BoneIdx : int {
    BONE_NONE = -1,
    BONE_HEAD = 0,
    BONE_NECK = 1,
    BONE_SPINE = 2,
    BONE_SPINE1 = 3,
    BONE_PELVIS = 4,
    BONE_LSHOULDER = 5,
    BONE_RSHOULDER = 6,
    BONE_LELBOW = 7,      // lforearm
    BONE_RELBOW = 8,      // rforearm
    BONE_LHAND = 9,
    BONE_RHAND = 10,
    BONE_LTHIGH = 11,
    BONE_RTHIGH = 12,
    BONE_LKNEE = 13,      // lknee (L Calf)
    BONE_RKNEE = 14,      // rknee (R Calf)
    BONE_LFOOT = 15,
    BONE_RFOOT = 16,
    BONE_COUNT = 17
};

// 🔥 FIX: Nama bone harus match dengan yang dikirim dari Python (biped ...)
static const char* BONE_NAMES[BONE_COUNT] = {
    "biped Head",        // BONE_HEAD
    "biped Neck",        // BONE_NECK
    "biped Spine",       // BONE_SPINE
    "biped Spine1",      // BONE_SPINE1
    "biped Pelvis",      // BONE_PELVIS
    "biped L UpperArm",  // BONE_LSHOULDER
    "biped R UpperArm",  // BONE_RSHOULDER
    "biped L Forearm",   // BONE_LELBOW
    "biped R Forearm",   // BONE_RELBOW
    "biped L Hand",      // BONE_LHAND
    "biped R Hand",      // BONE_RHAND
    "biped L Thigh",     // BONE_LTHIGH
    "biped R Thigh",     // BONE_RTHIGH
    "biped L Calf",      // BONE_LKNEE
    "biped R Calf",      // BONE_RKNEE
    "biped L Foot",      // BONE_LFOOT
    "biped R Foot"       // BONE_RFOOT
};

// 🔥 FIX: Short-name key yang persis dikirim Python di srcpy.cpp (_ESP_BONES)
// Ini yang bikin bone_cache gagal match sebelumnya karena Python kirim
// "head", "neck", dll — bukan "biped Head".
static const char* BONE_SHORT_NAMES[BONE_COUNT] = {
    "head",       // BONE_HEAD
    "neck",       // BONE_NECK
    "spine",      // BONE_SPINE
    "spine1",     // BONE_SPINE1
    "pelvis",     // BONE_PELVIS
    "lshoulder",  // BONE_LSHOULDER
    "rshoulder",  // BONE_RSHOULDER
    "lforearm",   // BONE_LELBOW
    "rforearm",   // BONE_RELBOW
    "lhand",      // BONE_LHAND
    "rhand",      // BONE_RHAND
    "lthigh",     // BONE_LTHIGH
    "rthigh",     // BONE_RTHIGH
    "lknee",      // BONE_LKNEE
    "rknee",      // BONE_RKNEE
    "lfoot",      // BONE_LFOOT
    "rfoot"       // BONE_RFOOT
};

// ============================================================================
// BONE CACHE
// ============================================================================
class BoneCache {
private:
    ImVec2 m_bones[BONE_COUNT];
    bool   m_has_bone[BONE_COUNT];
    int    m_valid_count;
    std::unordered_map<std::string, ImVec2> m_extra_bones;

public:
    BoneCache() : m_valid_count(0) { clear(); }

    void clear() {
        for (int i = 0; i < BONE_COUNT; i++) {
            m_bones[i] = ImVec2(0, 0);
            m_has_bone[i] = false;
        }
        m_extra_bones.clear();
        m_valid_count = 0;
    }

    bool isEmpty() const { return m_valid_count == 0; }

    void set(BoneIdx idx, float x, float y) {
        if (idx < 0 || idx >= BONE_COUNT) return;
        if (!isValidCoord(x) || !isValidCoord(y)) return;
        m_bones[idx] = ImVec2(x, y);
        if (!m_has_bone[idx]) { m_has_bone[idx] = true; m_valid_count++; }
    }

    // 🔥 FIX: Support ketiga format nama (short-key python, pendek, & panjang)
    void set(const std::string& name, float x, float y) {
        if (!isValidCoord(x) || !isValidCoord(y)) return;

        // Coba match ke short-key persis yang dikirim Python ("head","neck",dll)
        for (int i = 0; i < BONE_COUNT; i++) {
            if (name == BONE_SHORT_NAMES[i]) {
                set((BoneIdx)i, x, y);
                return;
            }
        }

        // Coba match langsung ke nama panjang
        for (int i = 0; i < BONE_COUNT; i++) {
            if (name == BONE_NAMES[i]) { 
                set((BoneIdx)i, x, y); 
                return; 
            }
        }
        
        // 🔥 FIX: Coba tanpa prefix "biped " (fallback)
        std::string shortName = name;
        if (name.rfind("biped ", 0) == 0) {
            shortName = name.substr(6);
        }
        for (int i = 0; i < BONE_COUNT; i++) {
            if (shortName == BONE_NAMES[i] || shortName == BONE_NAMES[i] + 1) {
                set((BoneIdx)i, x, y);
                return;
            }
        }
        
        // Fallback: simpan di extra
        m_extra_bones[name] = ImVec2(x, y);
        m_valid_count++;
    }

    bool get(BoneIdx idx, ImVec2& out) const {
        if (idx < 0 || idx >= BONE_COUNT) return false;
        if (!m_has_bone[idx]) return false;
        out = m_bones[idx]; 
        return true;
    }

    bool get(const std::string& name, ImVec2& out) const {
        // Coba match ke short-key persis yang dikirim Python ("head","neck",dll)
        for (int i = 0; i < BONE_COUNT; i++) {
            if (name == BONE_SHORT_NAMES[i]) {
                return get((BoneIdx)i, out);
            }
        }

        // Coba langsung
        for (int i = 0; i < BONE_COUNT; i++) {
            if (name == BONE_NAMES[i]) {
                return get((BoneIdx)i, out);
            }
        }
        
        // Coba tanpa prefix
        std::string shortName = name;
        if (name.rfind("biped ", 0) == 0) {
            shortName = name.substr(6);
        }
        for (int i = 0; i < BONE_COUNT; i++) {
            if (shortName == BONE_NAMES[i]) {
                return get((BoneIdx)i, out);
            }
        }
        
        // Cek extra
        auto it = m_extra_bones.find(name);
        if (it != m_extra_bones.end()) { 
            out = it->second; 
            return true; 
        }
        return false;
    }

    static bool isValidCoord(float v) {
        return !std::isnan(v) && !std::isinf(v) && v > -10000.f && v < 10000.f;
    }
};

// ============================================================================
// WORLD OBJECT
// ============================================================================
struct WorldObject {
    int         entity_id  = 0;
    std::string tag;
    std::string label;
    float       color[3]   = {1.f, 1.f, 1.f};
    float       worldpos[3]= {0.f, 0.f, 0.f};
    float       screen_x   = 0.f;
    float       screen_y   = 0.f;
    float       distance   = 0.f;
    int   airdrop_kind = -1;
    float veh_hp    = 0.f;
    float veh_maxhp = 0.f;
    int   veh_type  = 0;
    float obj_hp    = 0.f;
    float obj_maxhp = 0.f;
    std::chrono::steady_clock::time_point last_update;

    bool IsOnScreen() const {
        return screen_x > -200.f && screen_x < 10000.f &&
               screen_y > -200.f && screen_y < 10000.f;
    }
    ImVec2 ScreenPos() const { return ImVec2(screen_x, screen_y); }
    ImU32 GetColor(int alpha = 220) const {
        return IM_COL32((int)(color[0]*255), (int)(color[1]*255), (int)(color[2]*255), alpha);
    }
    std::string GetDistStr() const {
        char buf[24];
        if (distance >= 1000.f) snprintf(buf, sizeof(buf), "%.1fkm", distance/1000.f);
        else snprintf(buf, sizeof(buf), "%.0fm", distance);
        return std::string(buf);
    }
};

// ============================================================================
// ENTITY
// ============================================================================
struct Entity {
    std::string name;
    int         team_id = 0;
    int         hero_id = 0;
    bool        is_zombie = false;
    bool        is_bot = false;
    bool        is_teammate = false;
    std::vector<float> worldpos;
    float              distance = 0.f;
    bool               is_on_screen = false;
    bool               is_visible = false;
    std::unordered_map<std::string, std::vector<float>> bones;
    BoneCache bone_cache;
    float health = 100.f;
    float max_health = 100.f;
    float armor = 0.f;
    float base_armor = 0.f;
    float extra_armor = 0.f;
    bool is_alive = true;
    bool is_knocked = false;
    bool is_dead = false;
    bool is_invincible = false;
    int  pose_type = 0;
    bool  is_shield_active = false;
    float shield_hp = 0.f;
    float shield_maxhp = 100.f;
    int         weapon_id = 0;
    std::string weapon_name;
    int         weapon_gun_type = 0;
    int         optic_type = 0;
    std::vector<int> weapon_list;
    bool        is_reloading = false;
    bool is_ads = false;
    bool is_real_ads = false;
    bool is_shooting = false;
    bool is_sprint = false;
    bool is_super_sprint = false;
    bool is_fps_mode = false;
    bool is_on_sky = false;
    bool is_in_swim = false;
    bool is_veh_driver = false;
    bool is_veh_passenger = false;
    bool is_reviving = false;
    bool is_being_revived = false;
    float yaw = 0.f;
    float velocity_x = 0.f;
    float velocity_y = 0.f;
    float velocity_z = 0.f;
    float bone_depth[BONE_COUNT] = {0.f};
    std::string in_house;
    std::vector<int> buffs;
    std::string world_name;
    std::string real_world_name;
    bool is_moba_map = false;
    bool is_replaying = false;
    int  total_entities = 0;
    float radar_x = 0.f;
    float radar_y = 0.f;
    float radar_distance = 0.f;
    bool        is_item = false;
    int         item_id = 0;
    std::string item_label;
    int         item_count = 1;
    std::chrono::steady_clock::time_point last_update;

    float HpPercent() const {
        if (max_health <= 0.001f) return 0.f;
        float v = health / max_health;
        if (v < 0.f) return 0.f;
        if (v > 1.f) return 1.f;
        return std::isnan(v) || std::isinf(v) ? 0.f : v;
    }

    float ArmorPercent() const {
        if (armor <= 0.001f) return 0.f;
        float maxA = armor > 200.f ? 200.f : 100.f;
        float v = armor / maxA;
        return (v < 0.f || v > 1.f || std::isnan(v) || std::isinf(v)) ? 0.f : v;
    }

    float ShieldPercent() const {
        if (!is_shield_active || shield_maxhp <= 0.001f) return 0.f;
        float v = shield_hp / shield_maxhp;
        return (v < 0.f || v > 1.f) ? 0.f : v;
    }

    bool IsValid() const {
        if (is_item) return item_id != 0 && !item_label.empty();
        if (name.empty()) return false;
        if (!is_alive || is_dead) return false;
        if (worldpos.size() < 3) return false;
        for (int i = 0; i < 3; i++)
            if (std::isnan(worldpos[i]) || std::isinf(worldpos[i])) return false;
        return true;
    }

    bool IsWorldObject() const { return is_item; }

    // 🔥 FIX: Get bone dengan support kedua format nama
    bool GetBonePosition(BoneIdx bone, ImVec2& out) const {
        if (bone < 0 || bone >= BONE_COUNT) return false;
        
        // Coba dari bone_cache dulu
        if (bone_cache.get(bone, out)) return true;
        
        // Coba dari bones map dengan nama panjang
        const char* boneName = BONE_NAMES[bone];
        auto it = bones.find(boneName);
        if (it != bones.end() && it->second.size() >= 2) {
            float x = it->second[0], y = it->second[1];
            if (BoneCache::isValidCoord(x) && BoneCache::isValidCoord(y)) {
                out = ImVec2(x, y); 
                return true;
            }
        }
        
        // 🔥 FIX: Coba tanpa prefix "biped "
        std::string shortName = boneName;
        if (std::string(boneName).rfind("biped ", 0) == 0) {
            shortName = std::string(boneName).substr(6);
        }
        // Coba cari di bones
        for (const auto& pair : bones) {
            if (pair.first == shortName || pair.first.find(shortName) != std::string::npos) {
                if (pair.second.size() >= 2) {
                    float x = pair.second[0], y = pair.second[1];
                    if (BoneCache::isValidCoord(x) && BoneCache::isValidCoord(y)) {
                        out = ImVec2(x, y); 
                        return true;
                    }
                }
            }
        }
        
        return false;
    }

    float GetBoneDepth(BoneIdx bone) const {
        if (bone >= 0 && bone < BONE_COUNT) return bone_depth[bone];
        return 1.f;
    }

    void SetBoneWithDepth(BoneIdx idx, float x, float y, float depth) {
        bone_cache.set(idx, x, y);
        if (idx >= 0 && idx < BONE_COUNT) bone_depth[idx] = depth;
    }

    bool GetBonePositionByName(const std::string& boneName, ImVec2& out) const {
        if (bone_cache.get(boneName, out)) return true;
        auto it = bones.find(boneName);
        if (it != bones.end() && it->second.size() >= 2) {
            float x = it->second[0], y = it->second[1];
            if (BoneCache::isValidCoord(x) && BoneCache::isValidCoord(y)) {
                out = ImVec2(x, y); 
                return true;
            }
        }
        return false;
    }

    // 🔥 FIX: Rebuild bone cache dengan support kedua format
    void RebuildBoneCache() {
        bone_cache.clear();
        for (const auto& pair : bones) {
            if (pair.second.size() >= 2) {
                float x = pair.second[0], y = pair.second[1];
                if (BoneCache::isValidCoord(x) && BoneCache::isValidCoord(y)) {
                    // Set dengan nama asli (bone_cache.set udah handle kedua format)
                    bone_cache.set(pair.first, x, y);
                }
            }
        }
    }

    bool GetHeadScreenPos(ImVec2& out) const { 
        return GetBonePosition(BONE_HEAD, out); 
    }

    std::string GetDistanceString() const {
        if (distance <= 0) return "0m";
        char buf[16];
        if (distance >= 1000) snprintf(buf, sizeof(buf), "%.1fkm", distance/1000.f);
        else snprintf(buf, sizeof(buf), "%.0fm", distance);
        return std::string(buf);
    }

    Entity() { clear(); }

    void clear() {
        name.clear(); team_id = 0; hero_id = 0;
        is_zombie = false; is_bot = false; is_teammate = false;
        worldpos.clear(); distance = 0.f; is_on_screen = false; is_visible = false;
        bones.clear(); bone_cache.clear();
        health = 100.f; max_health = 100.f; armor = 0.f;
        base_armor = 0.f; extra_armor = 0.f;
        is_alive = true; is_knocked = false; is_dead = false;
        is_invincible = false; pose_type = 0;
        is_shield_active = false; shield_hp = 0.f; shield_maxhp = 100.f;
        weapon_id = 0; weapon_name.clear(); weapon_gun_type = 0; optic_type = 0;
        weapon_list.clear(); is_reloading = false;
        is_ads = false; is_real_ads = false; is_shooting = false;
        is_sprint = false; is_super_sprint = false; is_fps_mode = false;
        is_on_sky = false; is_in_swim = false;
        is_veh_driver = false; is_veh_passenger = false;
        is_reviving = false; is_being_revived = false;
        yaw = 0.f;
        velocity_x = velocity_y = velocity_z = 0.f;
        for (int i = 0; i < BONE_COUNT; i++) bone_depth[i] = 0.f;
        in_house.clear(); buffs.clear();
        world_name.clear(); real_world_name.clear();
        is_moba_map = false; is_replaying = false; total_entities = 0;
        radar_x = radar_y = radar_distance = 0.f;
        is_item = false; item_id = 0; item_label.clear(); item_count = 1;
    }

    Entity(const Entity& o) { copyFrom(o); }
    Entity& operator=(const Entity& o) { if (this != &o) copyFrom(o); return *this; }

private:
    void copyFrom(const Entity& o) {
        name = o.name; team_id = o.team_id; hero_id = o.hero_id;
        is_zombie = o.is_zombie; is_bot = o.is_bot; is_teammate = o.is_teammate;
        worldpos = o.worldpos; distance = o.distance;
        is_on_screen = o.is_on_screen; is_visible = o.is_visible;
        bones = o.bones;
        bone_cache.clear();
        // 🔥 FIX: Rebuild dengan method yang support kedua format
        for (const auto& p : bones) {
            if (p.second.size() >= 2) {
                bone_cache.set(p.first, p.second[0], p.second[1]);
            }
        }
        health = o.health; max_health = o.max_health; armor = o.armor;
        base_armor = o.base_armor; extra_armor = o.extra_armor;
        is_alive = o.is_alive; is_knocked = o.is_knocked; is_dead = o.is_dead;
        is_invincible = o.is_invincible; pose_type = o.pose_type;
        is_shield_active = o.is_shield_active; shield_hp = o.shield_hp; shield_maxhp = o.shield_maxhp;
        weapon_id = o.weapon_id; weapon_name = o.weapon_name;
        weapon_gun_type = o.weapon_gun_type; optic_type = o.optic_type;
        weapon_list = o.weapon_list; is_reloading = o.is_reloading;
        is_ads = o.is_ads; is_real_ads = o.is_real_ads; is_shooting = o.is_shooting;
        is_sprint = o.is_sprint; is_super_sprint = o.is_super_sprint;
        is_fps_mode = o.is_fps_mode; is_on_sky = o.is_on_sky; is_in_swim = o.is_in_swim;
        is_veh_driver = o.is_veh_driver; is_veh_passenger = o.is_veh_passenger;
        is_reviving = o.is_reviving; is_being_revived = o.is_being_revived;
        yaw = o.yaw;
        velocity_x = o.velocity_x; velocity_y = o.velocity_y; velocity_z = o.velocity_z;
        for (int i = 0; i < BONE_COUNT; i++) bone_depth[i] = o.bone_depth[i];
        in_house = o.in_house; buffs = o.buffs;
        world_name = o.world_name; real_world_name = o.real_world_name;
        is_moba_map = o.is_moba_map; is_replaying = o.is_replaying; total_entities = o.total_entities;
        radar_x = o.radar_x; radar_y = o.radar_y; radar_distance = o.radar_distance;
        is_item = o.is_item; item_id = o.item_id; item_label = o.item_label; item_count = o.item_count;
        last_update = o.last_update;
    }
};

// ============================================================================
// HELPER FUNCTIONS
// ============================================================================
static inline bool IsValidEntity(const Entity& e) { return e.IsValid(); }
static inline bool IsEnemy(const Entity& e) { 
    return !e.is_teammate && !e.is_item && e.is_alive && !e.is_dead; 
}
static inline bool IsKnockedEnemy(const Entity& e) { 
    return IsEnemy(e) && e.is_knocked; 
}
static inline bool IsAliveEnemy(const Entity& e) { 
    return IsEnemy(e) && !e.is_knocked; 
}

