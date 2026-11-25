#pragma once
#ifndef IMCOLOR_DEFINED
#define IMCOLOR_DEFINED
#endif
#include <string>
#include <vector>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <atomic>
#include <chrono>
#include "imgui/imgui.h"
#include "include/sync_entities.h"
#include "entity.h"
#include "ivon.h"

#ifdef WHITE
#undef WHITE
#endif
#ifdef BLACK
#undef BLACK
#endif

extern int    Width;
extern int    Height;
extern float  global_anim_time;
extern std::atomic<bool> menu_visible;
extern void AddNotification(const char* msg, int r, int g, int b, float dur);

namespace ESP {
// ==================== COLORS ====================
namespace Colors {
    // 🔥 WARNA MUSUH TERLIHAT (JELAS & TERANG)
    static constexpr ImU32 COLOR_VISIBLE      = IM_COL32(255, 255, 255, 255);
    static constexpr ImU32 COLOR_VISIBLE_GLOW = IM_COL32(255, 255, 255, 60);
    
    // 🔥 WARNA MUSUH DI BALIK TEMBOK (REDUP & ABU-ABU)
    static constexpr ImU32 COLOR_HIDDEN       = IM_COL32(150, 150, 150, 100);
    static constexpr ImU32 COLOR_HIDDEN_BG    = IM_COL32(50,  50,  50,  150);
    
    // 🔥 WARNA KNOCK (MERAH TERANG, PALING MENCOLOK)
    static constexpr ImU32 COLOR_KNOCK        = IM_COL32(255, 50,  50,  255);
    static constexpr ImU32 COLOR_KNOCK_BG     = IM_COL32(255, 50,  50,  200);
    
    static constexpr ImU32 COLOR_BOT          = IM_COL32(100, 180, 255, 255);
    static constexpr ImU32 COLOR_ZOMBIE       = IM_COL32(86,  214, 130, 255);
    static constexpr ImU32 COLOR_GREEN        = IM_COL32(74,  222, 128, 255);
    static constexpr ImU32 COLOR_AI_BG        = IM_COL32(20,  90,  210, 235);
    static constexpr ImU32 COLOR_PANEL_BG     = IM_COL32(10,  10,  14,  220);
    
    static inline ImU32 Rainbow(float t) {
        float h = t - 0.5f; // Offset biar merah mulai dari atas
        h -= floorf(h);
        ImVec4 c = (ImVec4)ImColor::HSV(h, 0.85f, 1.0f);
        return IM_COL32((int)(c.x * 255), (int)(c.y * 255), (int)(c.z * 255), 255);
    }
}

// ==================== FRAME FLAGS ====================
struct FrameFlags {
    bool  bESP         = true;
    bool  bBox         = true;
    bool  bHealth      = true;
    bool  bDistance    = true;
    bool  bName        = true;
    bool  bWeapon      = true;
    bool  bSkeleton    = true;
    bool  bLane        = true;
    bool  bRadar       = false;
    bool  bAIM         = false;
    bool  bAimLane     = true;
    bool  bLootESP     = false;
    float fMaxDist     = 300.f;
    float fRadarSize   = 150.f;
    float fRadarRange  = 400.f;
    float fAimFov      = 60.f;
    float fAimMaxDist  = 300.f;
    float fLootMaxDist = 120.f;
};
static FrameFlags s_ff;
static ImFont*    s_font = nullptr;

// ==================== HELPERS ====================
// 🔥 UKURAN TETAP: Jauh atau dekat, ukuran font & garis SAMA PERSIS
static inline float _ts(float dist, float base = 12.f) {
    return base; 
}

static inline bool sg(const char* k, bool def) {
    auto it = sync_bool.find(k);
    return it != sync_bool.end() ? it->second : def;
}
static inline float sf(const char* k, float def) {
    auto it = sync_float.find(k);
    return it != sync_float.end() ? it->second : def;
}

static void SnapshotFlags() {
    s_ff.bESP        = sg("bESP", true);
    s_ff.bBox        = sg("bESP_Box", true);
    s_ff.bHealth     = sg("bESP_Health", true);
    s_ff.bDistance   = sg("bESP_Distance", true);
    s_ff.bName       = sg("bESP_Name", true);
    s_ff.bWeapon     = sg("bESP_Weapon", true);
    s_ff.bSkeleton   = sg("bESP_Skeleton", true);
    s_ff.bLane       = sg("bESP_Lane", true);
    s_ff.bRadar      = sg("bESP_Radar", false);
    s_ff.bAIM        = sg("bAIM", false);
    s_ff.bAimLane    = sg("bAIM_Lane", true);
    s_ff.bLootESP    = sg("bLootESP", false);
    s_ff.fMaxDist    = sf("fESP_MaxDistance", 300.f);
    s_ff.fRadarSize  = sf("fESP_RadarSize", 150.f);
    s_ff.fRadarRange = sf("fESP_RadarRange", 400.f);
    s_ff.fAimFov     = sf("fAIM_Fov", 60.f);
    s_ff.fAimMaxDist = sf("fAIM_MaxDist", 300.f);
    s_ff.fLootMaxDist = sf("fLootESP_MaxDist", 120.f);
}

// ==================== UTILS ====================
namespace Utils {
    static inline bool IsValidFloat(float v) { return !std::isnan(v) && !std::isinf(v) && v > -10000.f && v < 10000.f; }
    static inline bool IsValidVec2(const ImVec2& v) { return IsValidFloat(v.x) && IsValidFloat(v.y); }
    static inline ImFont* GetSafeFont() {
        ImGuiIO& io = ImGui::GetIO();
        if (!io.Fonts || io.Fonts->Fonts.Size == 0 || !io.Fonts->Fonts[0]) return nullptr;
        return io.Fonts->Fonts[0];
    }
    static inline bool IsInBounds(const ImVec2& p) {
        if (!IsValidVec2(p)) return false;
        return p.x >= -100.f && p.x <= (float)Width + 100.f && p.y >= -100.f && p.y <= (float)Height + 100.f;
    }
    static inline void Text(ImDrawList* dl, float x, float y, ImU32 col, const char* txt, float size = 12.f) {
        if (!dl || !txt || !txt[0]) return;
        ImFont* font = GetSafeFont();
        if (!font) { dl->AddText(ImVec2(x, y), col, txt); return; }
        dl->AddText(font, size, ImVec2(x + 1, y + 1), IM_COL32(0, 0, 0, 180), txt);
        dl->AddText(font, size, ImVec2(x, y), col, txt);
    }
    static inline void TextCentered(ImDrawList* dl, float cx, float y, ImU32 col, const char* txt, float size = 12.f) {
        if (!dl || !txt || !txt[0]) return;
        float w = s_font ? s_font->CalcTextSizeA(size, FLT_MAX, 0.f, txt).x : ImGui::CalcTextSize(txt).x;
        Text(dl, cx - w * 0.5f, y, col, txt, size);
    }
    static inline void RectFilled(ImDrawList* dl, const ImVec2& a, const ImVec2& b, ImU32 col, float r = 0.f) {
        if (!dl || !IsValidVec2(a) || !IsValidVec2(b)) return;
        if (a.x > b.x || a.y > b.y) return;
        dl->AddRectFilled(a, b, col, r);
    }
    static inline void Line(ImDrawList* dl, const ImVec2& a, const ImVec2& b, ImU32 col, float thickness = 1.f) {
        if (!dl || !IsValidVec2(a) || !IsValidVec2(b)) return;
        dl->AddLine(a, b, col, thickness);
    }
}

// ==================== BONE HELPER ====================
namespace BoneHelper {
    static bool GetFootPos(const Entity& e, const ImVec2& head, ImVec2& foot) {
        if (!Utils::IsValidVec2(head) || head.y <= 0.f) return false;
        ImVec2 lf, rf, sp;
        if (e.GetBonePosition(BONE_LFOOT, lf) && Utils::IsInBounds(lf)) { foot = lf; return true; }
        if (e.GetBonePosition(BONE_RFOOT, rf) && Utils::IsInBounds(rf)) { foot = rf; return true; }
        if (e.GetBonePosition(BONE_SPINE, sp) && Utils::IsInBounds(sp)) {
            foot = ImVec2(head.x, sp.y + (sp.y - head.y) * 0.5f);
            return true;
        }
        float d = std::max(e.distance, 1.f);
        float estH = (50.f * 70.f) / d;
        estH = std::clamp(estH, 8.f, 260.f);
        foot = ImVec2(head.x, head.y + estH);
        return true;
    }
}

// ==================== 1. ESP COUNT ====================
namespace ESPCount {
    static void Draw(ImDrawList* dl, const std::vector<Entity>& ents) {
        if (!dl) return;
        int players = 0, bots = 0;
        for (const auto& e : ents) {
            if (e.IsWorldObject() || !e.is_alive || e.is_teammate) continue;
            if (e.distance > s_ff.fMaxDist) continue;
            if (e.is_bot) bots++; else players++;
        }
        char buf[64];
        snprintf(buf, sizeof(buf), "Players: %d  |  Bots: %d", players, bots);
        float fs = 13.f;
        float cx = Width * 0.5f;
        float cy = 15.f;
        float tw = s_font ? s_font->CalcTextSizeA(fs, FLT_MAX, 0.f, buf).x : ImGui::CalcTextSize(buf).x;
        float pad = 8.f;
        Utils::RectFilled(dl, ImVec2(cx - tw/2 - pad, cy - pad/2), ImVec2(cx + tw/2 + pad, cy + fs + pad/2 + 2), IM_COL32(0, 0, 0, 160), 6.f);
        Utils::TextCentered(dl, cx, cy, Colors::COLOR_VISIBLE, buf, fs);
    }
}

// ==================== 2. BOTTOM INFO (REQUESTED) ====================
namespace BottomInfo {
    static void Draw(ImDrawList* dl) {
        if (!dl || !s_font) return;
        float fs = 14.f; // Font agak besar biar jelas
        float x = 15.f, y = Height - 95.f;
        float gap = 22.f;
        
        // Background panel biar teks kebaca di background game apapun
        Utils::RectFilled(dl, ImVec2(x - 8.f, y - 8.f), ImVec2(x + 260.f, y + 3 * gap + 8.f), IM_COL32(0, 0, 0, 180), 8.f);
        
        // 1. Nexora (Warna Accent Biru Terang)
        Utils::Text(dl, x, y, IM_COL32(0, 180, 255, 255), "Nexora", fs + 2.f);
        
        // 2. Telegram Link (Warna Putih/Abu)
        Utils::Text(dl, x, y + gap, IM_COL32(220, 220, 220, 230), "https://t.me/ShorekeeperID", fs);
        
        // 3. Best Cheat (Warna Hijau Neon)
        Utils::Text(dl, x, y + gap * 2.f, Colors::COLOR_GREEN, "Best Cheat", fs);
    }
}

// ==================== 3. ESP LANE (Top Anchor) ====================
namespace ESPLane {
    static void Draw(ImDrawList* dl, const std::vector<Entity>& ents) {
        if (!dl || !s_ff.bLane) return;
        ImVec2 anchor(Width * 0.5f, 10.f);
        for (const auto& e : ents) {
            if (e.IsWorldObject() || !e.is_alive || e.is_teammate) continue;
            if (e.distance > s_ff.fMaxDist) continue;
            ImVec2 head;
            if (e.GetBonePosition(BONE_HEAD, head) && Utils::IsInBounds(head)) {
                // 🔥 JELAS: Terlihat = Putih, Di balik tembok = Abu-abu redup
                ImU32 laneCol = e.is_visible ? IM_COL32(255, 255, 255, 180) : IM_COL32(150, 150, 150, 80);
                Utils::Line(dl, anchor, head, laneCol, 1.f);
            }
        }
    }
}

// ==================== 4. AIM LANE (Bottom Anchor - VISIBLE ONLY) ====================
namespace AimLane {
    static void Draw(ImDrawList* dl, const std::vector<Entity>& ents) {
        if (!dl || !s_ff.bAIM || !s_ff.bAimLane) return;
        
        const Entity* bestTarget = nullptr;
        float bestDist = s_ff.fAimMaxDist;
        
        for (const auto& e : ents) {
            if (e.IsWorldObject() || !e.is_alive || e.is_teammate) continue;
            
            // 🔥 CRITICAL: AIM LANE TIDAK AKAN MUNCUL JIKA DI BALIK TEMBOK
            if (!e.is_visible) continue; 
            
            if (e.distance < bestDist) {
                bestDist = e.distance;
                bestTarget = &e;
            }
        }
        
        if (bestTarget) {
            ImVec2 anchor(Width * 0.5f, Height - 10.f);
            ImVec2 head;
            if (bestTarget->GetBonePosition(BONE_HEAD, head) && Utils::IsInBounds(head)) {
                // Warna Cyan Terang biar beda jelas sama ESP Lane
                Utils::Line(dl, anchor, head, IM_COL32(0, 255, 255, 255), 1.8f);
            }
        }
    }
}

// ==================== 5. NAME PANEL (WITH KNOCK & WALL INDICATOR) ====================
namespace NamePanel {
    static void Draw(ImDrawList* dl, const Entity& e, const ImVec2& head) {
        if (!dl || !s_font) return;
        if (!s_ff.bName && !s_ff.bDistance) return;
        
        float fs = _ts(e.distance, 12.f);
        char distBuf[16];
        if (e.distance >= 1000.f) snprintf(distBuf, sizeof(distBuf), "%.1fkm", e.distance / 1000.f);
        else                      snprintf(distBuf, sizeof(distBuf), "%.0fm", e.distance);
        
        std::string nameStr;
        if (!e.name.empty()) nameStr = e.name.length() > 14 ? e.name.substr(0, 13) + "." : e.name;
        else nameStr = e.is_bot ? "robot" : "player";
        
        // 🔥 LOGIKA WARNA & TEKS BERDASARKAN KONDISI
        const char* aiTag;
        ImU32 tagBg;
        ImU32 textColor;
        
        if (e.is_knocked) {
            aiTag = "KNOCK";
            tagBg = Colors::COLOR_KNOCK_BG;
            textColor = Colors::COLOR_VISIBLE;
        } else if (!e.is_visible) {
            aiTag = "[W]"; // Indikator Di Balik Tembok
            tagBg = Colors::COLOR_HIDDEN_BG;
            textColor = Colors::COLOR_HIDDEN;
        } else {
            aiTag = e.is_bot ? "AI" : (e.is_zombie ? "ZB" : "PL");
            tagBg = Colors::COLOR_AI_BG;
            textColor = Colors::COLOR_VISIBLE;
        }
        
        float padX = 8.f, padY = 4.f;
        float segGap = 6.f;
        float aiW   = s_font->CalcTextSizeA(fs, FLT_MAX, 0.f, aiTag).x;
        float nameW = s_font->CalcTextSizeA(fs, FLT_MAX, 0.f, nameStr.c_str()).x;
        float distW = s_font->CalcTextSizeA(fs, FLT_MAX, 0.f, distBuf).x;
        
        float aiBoxW   = aiW   + padX * 2.f;
        float nameBoxW = nameW + padX * 2.f;
        float distBoxW = distW + padX * 2.f;
        float rowH     = fs + padY * 2.f;
        float totalW   = aiBoxW + nameBoxW + distBoxW + segGap * 2.f;
        
        float bx = head.x - totalW * 0.5f;
        float by = head.y - rowH - 8.f;
        
        if (!Utils::IsInBounds(ImVec2(bx, by))) return;
        
        
 
        
        // Background Panel Utama
        ImU32 mainBg = e.is_visible ? Colors::COLOR_PANEL_BG : Colors::COLOR_HIDDEN_BG;
        Utils::RectFilled(dl, ImVec2(bx, by), ImVec2(bx + totalW, by + rowH), mainBg, 4.f);
        
        // Background Tag (AI / KNOCK / [W])
        Utils::RectFilled(dl, ImVec2(bx, by), ImVec2(bx + aiBoxW, by + rowH), tagBg, 4.f);
        
        // Texts
        Utils::TextCentered(dl, bx + aiBoxW * 0.5f, by + padY - 1.f, Colors::COLOR_VISIBLE, aiTag, fs);
        Utils::TextCentered(dl, bx + aiBoxW + segGap + nameBoxW * 0.5f, by + padY - 1.f, textColor, nameStr.c_str(), fs);
        Utils::TextCentered(dl, bx + aiBoxW + segGap + nameBoxW + segGap + distBoxW * 0.5f, by + padY - 1.f, textColor, distBuf, fs);
        
        // HP + Armor Bar
        if (s_ff.bHealth) {
            float hp = e.HpPercent();
            float armor = e.ArmorPercent();
            float total = std::min(1.0f, hp + armor);
            float lineY = by + rowH + 2.f;
            float barH = 2.5f;
            
            Utils::RectFilled(dl, ImVec2(bx, lineY), ImVec2(bx + totalW, lineY + barH), IM_COL32(0,0,0,150), 1.f);
            // Bar jadi merah kalau knock, hijau kalau normal
            ImU32 barCol = e.is_knocked ? Colors::COLOR_KNOCK : Colors::COLOR_GREEN;
            Utils::RectFilled(dl, ImVec2(bx, lineY), ImVec2(bx + totalW * total, lineY + barH), barCol, 1.f);
        }
    }
}

// ==================== 6. WEAPON ESP ====================
namespace WeaponESP {
    static void Draw(ImDrawList* dl, const Entity& e, const ImVec2& foot) {
        if (!dl || !s_font) return;
        if (!s_ff.bWeapon) return;
        if (e.weapon_name.empty() || e.weapon_name == "Punk" || e.weapon_name == "Unknown") return;
        
        float fs = _ts(e.distance, 10.f);
        const char* wName = e.weapon_name.c_str();
        float tw = s_font->CalcTextSizeA(fs, FLT_MAX, 0.f, wName).x;
        float x = foot.x - tw * 0.5f;
        float y = foot.y + 4.f;
        
        // Background lebih transparan kalau di balik tembok
        ImU32 bgCol = e.is_visible ? IM_COL32(0, 0, 0, 160) : IM_COL32(0, 0, 0, 100);
        Utils::RectFilled(dl, ImVec2(x - 6.f, y - 1.f), ImVec2(x + tw + 6.f, y + fs + 3.f), bgCol, 6.f);
        
        ImU32 wCol = e.is_visible ? IM_COL32(255, 215, 60, 220) : IM_COL32(150, 150, 150, 150);
        Utils::Text(dl, x + 1, y + 1, IM_COL32(0, 0, 0, 180), wName, fs);
        Utils::Text(dl, x, y, wCol, wName, fs);
    }
}

// ==================== 7. SKELETON ====================
namespace SkeletonESP {
    static void Draw(ImDrawList* dl, const Entity& e) {
        if (!s_ff.bSkeleton) return;
        if (e.distance > 150.f) return;
        
        // 🔥 JELAS: Terlihat = Putih Tebal, Di balik tembok = Abu-abu Tipis & Transparan
        ImU32 col = e.is_visible ? Colors::COLOR_VISIBLE : Colors::COLOR_HIDDEN;
        float thick = e.is_visible ? 1.5f : 1.0f;
        
        static constexpr BoneIdx full[][2] = {
            {BONE_HEAD, BONE_NECK}, {BONE_NECK, BONE_SPINE}, {BONE_SPINE, BONE_SPINE1}, 
            {BONE_SPINE1, BONE_PELVIS},
            {BONE_NECK, BONE_LSHOULDER}, {BONE_LSHOULDER, BONE_LELBOW}, {BONE_LELBOW, BONE_LHAND},
            {BONE_NECK, BONE_RSHOULDER}, {BONE_RSHOULDER, BONE_RELBOW}, {BONE_RELBOW, BONE_RHAND},
            {BONE_PELVIS, BONE_LTHIGH}, {BONE_LTHIGH, BONE_LKNEE}, {BONE_LKNEE, BONE_LFOOT},
            {BONE_PELVIS, BONE_RTHIGH}, {BONE_RTHIGH, BONE_RKNEE}, {BONE_RKNEE, BONE_RFOOT},
        };
        int cnt = e.distance < 40.f ? 15 : e.distance < 80.f ? 9 : 6;
        ImVec2 a, b;
        for (int i = 0; i < cnt; i++) {
            if (!e.GetBonePosition(full[i][0], a) || !Utils::IsInBounds(a)) continue;
            if (!e.GetBonePosition(full[i][1], b) || !Utils::IsInBounds(b)) continue;
            Utils::Line(dl, a, b, col, thick);
        }
        ImVec2 headP;
        if (e.GetBonePosition(BONE_HEAD, headP) && Utils::IsInBounds(headP)) {
            Utils::RectFilled(dl, ImVec2(headP.x - 2.f, headP.y - 2.f), ImVec2(headP.x + 2.f, headP.y + 2.f), col);
        }
    }
}

// ==================== 8. AIM ESP (FOV CIRCLE) ====================
namespace AimESP {
    static void DrawFovCircle(ImDrawList* dl) {
        if (!dl || !s_ff.bAIM) return;
        
        // 🔥 LANGSUNG PAKAI NILAI PIXEL DARI SLIDER (SAMA PERSIS DENGAN PYTHON)
        // Python: max_fov = min(sync_attrs.get('fAIM_Fov', 180.0), 400.0)
        float r = sf("fAIM_Fov", 60.f); 
        r = std::clamp(r, 10.f, 800.f); // Batas aman
        
        float cx = Width * 0.5f, cy = Height * 0.5f;
        const int segs = 96;
        
        // Main Rainbow Circle
        for (int i = 0; i < segs; i++) {
            float a0 = (float)i     / segs * 6.28318530718f - 1.57079632679f;
            float a1 = (float)(i+1) / segs * 6.28318530718f - 1.57079632679f;
            ImVec2 p0(cx + cosf(a0) * r, cy + sinf(a0) * r);
            ImVec2 p1(cx + cosf(a1) * r, cy + sinf(a1) * r);
            ImU32 col = Colors::Rainbow((float)i / segs);
            Utils::Line(dl, p0, p1, col, 2.2f);
        }
        // Glow Circle
        for (int i = 0; i < segs; i++) {
            float a0 = (float)i     / segs * 6.28318530718f - 1.57079632679f;
            float a1 = (float)(i+1) / segs * 6.28318530718f - 1.57079632679f;
            ImVec2 p0(cx + cosf(a0) * (r + 3.f), cy + sinf(a0) * (r + 3.f));
            ImVec2 p1(cx + cosf(a1) * (r + 3.f), cy + sinf(a1) * (r + 3.f));
            ImU32 col = Colors::Rainbow((float)i / segs) & 0x30FFFFFF;
            Utils::Line(dl, p0, p1, col, 4.f);
        }
    }
}

// ==================== 9. LOOT ESP ====================
namespace LootItemESP {
    static void Draw(ImDrawList* dl, const std::vector<Entity>& ents) {
        if (!dl || !s_font || !s_ff.bLootESP) return;
        for (const auto& e : ents) {
            if (!e.IsWorldObject()) continue;
            if (e.distance > s_ff.fLootMaxDist) continue;
            if (!e.is_on_screen) continue;

            ImVec2 pos;
            if (!e.GetBonePosition(BONE_HEAD, pos) || !Utils::IsInBounds(pos)) continue;

            std::string label = e.item_label.empty() ? "Item" : e.item_label;
            if (e.item_count > 1) {
                char buf[160];
                snprintf(buf, sizeof(buf), "%s x%d", label.c_str(), e.item_count);
                label = buf;
            }
            std::string distStr = e.GetDistanceString();
            std::string full = label + "  [" + distStr + "]";

            float fs = 12.f;
            float tw = s_font->CalcTextSizeA(fs, FLT_MAX, 0.f, full.c_str()).x;
            float pad = 5.f;
            Utils::RectFilled(dl, ImVec2(pos.x - tw/2 - pad, pos.y - fs/2 - pad/2),
                               ImVec2(pos.x + tw/2 + pad, pos.y + fs/2 + pad/2), IM_COL32(0, 0, 0, 160), 4.f);
            Utils::TextCentered(dl, pos.x, pos.y - fs/2, IM_COL32(255, 215, 0, 255), full.c_str(), fs);
        }
    }
}

// ==================== MAIN DRAW ====================
inline void DrawAll(ImDrawList* dl, const std::vector<Entity>& ents) {
    if (!dl) return;
    if (Width <= 0 || Height <= 0) return;
    s_font = Utils::GetSafeFont();
    SnapshotFlags();
    if (!s_ff.bESP) return;
    
    ESPCount::Draw(dl, ents);
    BottomInfo::Draw(dl);
    ESPLane::Draw(dl, ents);
    AimLane::Draw(dl, ents);
    AimESP::DrawFovCircle(dl);
    LootItemESP::Draw(dl, ents);
    
    for (const auto& e : ents) {
        if (e.IsWorldObject() || !e.is_alive || e.is_teammate) continue;
        if (!e.is_on_screen || e.bone_cache.isEmpty()) continue;
        if (e.distance > s_ff.fMaxDist) continue;
        
        ImVec2 head, foot;
        if (!e.GetBonePosition(BONE_HEAD, head) || !Utils::IsInBounds(head)) continue;
        if (!BoneHelper::GetFootPos(e, head, foot)) continue;
        if (head.y < 0.f || head.y > (float)Height + 100.f || foot.y < head.y) continue;
        
        NamePanel::Draw(dl, e, head);
        WeaponESP::Draw(dl, e, foot);
        SkeletonESP::Draw(dl, e);
    }
}
} // namespace ESP
