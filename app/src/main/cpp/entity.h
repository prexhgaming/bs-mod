#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <cmath>
#include <cstdio>
#include "imgui/imgui.h"

// ============================================================================
// BONE INDEX — must match the key order of _ESP_BONES in srcpy.cpp
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
    BONE_LELBOW = 7,
    BONE_RELBOW = 8,
    BONE_LHAND = 9,
    BONE_RHAND = 10,
    BONE_LTHIGH = 11,
    BONE_RTHIGH = 12,
    BONE_LKNEE = 13,
    BONE_RKNEE = 14,
    BONE_LFOOT = 15,
    BONE_RFOOT = 16,
    BONE_COUNT = 17
};

// Short keys, exactly as srcpy.cpp sends them ("head", "neck", ...).
static const char* BONE_SHORT_NAMES[BONE_COUNT] = {
    "head", "neck", "spine", "spine1", "pelvis",
    "lshoulder", "rshoulder", "lforearm", "rforearm",
    "lhand", "rhand", "lthigh", "rthigh",
    "lknee", "rknee", "lfoot", "rfoot"
};

// Engine-side bone names, kept as a fallback for the long form.
static const char* BONE_NAMES[BONE_COUNT] = {
    "biped Head", "biped Neck", "biped Spine", "biped Spine1", "biped Pelvis",
    "biped L UpperArm", "biped R UpperArm", "biped L Forearm", "biped R Forearm",
    "biped L Hand", "biped R Hand", "biped L Thigh", "biped R Thigh",
    "biped L Calf", "biped R Calf", "biped L Foot", "biped R Foot"
};

// ============================================================================
// BONE CACHE — screen-space bones, resolved once per update
// ============================================================================
class BoneCache {
private:
    ImVec2 m_bones[BONE_COUNT];
    bool   m_has_bone[BONE_COUNT];
    int    m_valid_count;

public:
    BoneCache() : m_valid_count(0) { clear(); }

    void clear() {
        for (int i = 0; i < BONE_COUNT; i++) {
            m_bones[i] = ImVec2(0, 0);
            m_has_bone[i] = false;
        }
        m_valid_count = 0;
    }

    bool isEmpty() const { return m_valid_count == 0; }

    void set(BoneIdx idx, float x, float y) {
        if (idx < 0 || idx >= BONE_COUNT) return;
        if (!isValidCoord(x) || !isValidCoord(y)) return;
        m_bones[idx] = ImVec2(x, y);
        if (!m_has_bone[idx]) { m_has_bone[idx] = true; m_valid_count++; }
    }

    // Resolve a bone by name. Only the names in BONE_SHORT_NAMES / BONE_NAMES
    // are ever sent, so an unknown name is simply ignored.
    void set(const std::string& name, float x, float y) {
        int idx = indexOf(name);
        if (idx >= 0) set((BoneIdx)idx, x, y);
    }

    bool get(BoneIdx idx, ImVec2& out) const {
        if (idx < 0 || idx >= BONE_COUNT) return false;
        if (!m_has_bone[idx]) return false;
        out = m_bones[idx];
        return true;
    }

    bool get(const std::string& name, ImVec2& out) const {
        int idx = indexOf(name);
        return idx >= 0 ? get((BoneIdx)idx, out) : false;
    }

    static bool isValidCoord(float v) {
        return !std::isnan(v) && !std::isinf(v) && v > -10000.f && v < 10000.f;
    }

private:
    static int indexOf(const std::string& name) {
        for (int i = 0; i < BONE_COUNT; i++)
            if (name == BONE_SHORT_NAMES[i]) return i;
        for (int i = 0; i < BONE_COUNT; i++)
            if (name == BONE_NAMES[i]) return i;
        return -1;
    }
};

// ============================================================================
// ENTITY — only the fields srcpy.cpp actually sends and the overlay draws
// ============================================================================
struct Entity {
    // player identity / state
    std::string name;
    bool        is_zombie   = false;
    bool        is_bot      = false;
    bool        is_teammate = false;
    bool        is_alive    = true;
    bool        is_dead     = false;
    bool        is_knocked  = false;

    // projection
    float       distance     = 0.f;
    bool        is_on_screen = false;
    bool        is_visible   = false;
    std::unordered_map<std::string, std::vector<float>> bones;
    BoneCache   bone_cache;

    // bars / labels
    float       health     = 100.f;
    float       max_health = 100.f;
    float       armor      = 0.f;
    std::string weapon_name;

    // loot ESP (bLootESP)
    bool        is_item    = false;
    std::string item_label;
    int         item_count = 1;

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

    // The script sends no world position — everything is drawn from bones — so
    // validity is only "do we have something we can label and place".
    bool IsValid() const {
        if (is_item) return !item_label.empty();
        if (name.empty()) return false;
        if (!is_alive || is_dead) return false;
        return true;
    }

    bool IsWorldObject() const { return is_item; }

    bool GetBonePosition(BoneIdx bone, ImVec2& out) const {
        if (bone < 0 || bone >= BONE_COUNT) return false;
        if (bone_cache.get(bone, out)) return true;

        auto lookup = [&](const char* key) -> bool {
            auto it = bones.find(key);
            if (it == bones.end() || it->second.size() < 2) return false;
            float x = it->second[0], y = it->second[1];
            if (!BoneCache::isValidCoord(x) || !BoneCache::isValidCoord(y)) return false;
            out = ImVec2(x, y);
            return true;
        };
        return lookup(BONE_SHORT_NAMES[bone]) || lookup(BONE_NAMES[bone]);
    }

    void RebuildBoneCache() {
        bone_cache.clear();
        for (const auto& pair : bones) {
            if (pair.second.size() >= 2)
                bone_cache.set(pair.first, pair.second[0], pair.second[1]);
        }
    }

    std::string GetDistanceString() const {
        if (distance <= 0) return "0m";
        char buf[16];
        if (distance >= 1000) snprintf(buf, sizeof(buf), "%.1fkm", distance / 1000.f);
        else snprintf(buf, sizeof(buf), "%.0fm", distance);
        return std::string(buf);
    }

    Entity() { clear(); }

    void clear() {
        name.clear();
        is_zombie = false; is_bot = false; is_teammate = false;
        is_alive = true; is_dead = false; is_knocked = false;
        distance = 0.f; is_on_screen = false; is_visible = false;
        bones.clear(); bone_cache.clear();
        health = 100.f; max_health = 100.f; armor = 0.f;
        weapon_name.clear();
        is_item = false; item_label.clear(); item_count = 1;
    }

    Entity(const Entity& o) { copyFrom(o); }
    Entity& operator=(const Entity& o) { if (this != &o) copyFrom(o); return *this; }

private:
    void copyFrom(const Entity& o) {
        name = o.name;
        is_zombie = o.is_zombie; is_bot = o.is_bot; is_teammate = o.is_teammate;
        is_alive = o.is_alive; is_dead = o.is_dead; is_knocked = o.is_knocked;
        distance = o.distance;
        is_on_screen = o.is_on_screen; is_visible = o.is_visible;
        bones = o.bones;
        bone_cache = o.bone_cache;
        health = o.health; max_health = o.max_health; armor = o.armor;
        weapon_name = o.weapon_name;
        is_item = o.is_item; item_label = o.item_label; item_count = o.item_count;
    }
};
