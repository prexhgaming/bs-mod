#pragma once
#include "AImGui/AImGui.hpp"
#include "imgui/backends/imgui_impl_opengl3.h"
#include <string>
#include <atomic>
#include <cmath>
#include <cstdio>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <ctime>
#include <chrono>
#include "include/Jni_Stuff.h"
#include "SkinInfoSync.h"
#include "Colors.h"

extern void AddNotification(const char* msg, int r, int g, int b, float dur);
extern float global_anim_time;
extern GLuint g_logo_tex;
extern std::atomic<bool> HIDEESP;
extern int Width, Height;
extern std::unordered_map<std::string, bool>  sync_bool;
extern std::unordered_map<std::string, float> sync_float;
extern std::unordered_map<std::string, int>   sync_int;
extern void sync_with_py();
extern void SetDefaultConfig();
extern void SaveConfig();
extern void LoadConfig();
extern bool LoadMandarinFontOnDemand();
extern std::atomic<bool> menu_visible;
extern ImVec2 windowPos;
extern bool g_pending_cjk_reload;
extern bool g_cjkLoaded;

#define APP_VERSION "V25 VIP"
#define APP_MAKER   "NEXORA"
#define APP_CHANNEL "NEXORA"

static int g_themeIdx = 0;

struct WeaponInfo { int id; const char* cat; };
static const WeaponInfo WEAPONS[] = {
    {1,"AR"},{40,"AR"},{88,"AR"},{72,"AR"},{91,"AR"},{93,"AR"},{98,"AR"},{101,"AR"},
    {102,"AR"},{114,"AR"},{115,"AR"},{121,"AR"},{146,"AR"},{142,"AR"},
    {77,"SNI"},{103,"SNI"},{71,"SNI"},{122,"SNI"},{83,"SNI"},
    {34,"SHT"},{79,"SHT"},{126,"SHT"},{139,"SHT"},
    {2,"SMG"},{129,"SMG"},{38,"SMG"},{75,"SMG"},{76,"SMG"},{90,"SMG"},
    {110,"SMG"},{140,"SMG"},{144,"SMG"},
    {15,"PST"},{74,"PST"},{100,"PST"},
    {108,"LMG"},{82,"LMG"},{94,"LMG"},{125,"LMG"},
    {33,"MEL"},{86,"MEL"},{84,"MEL"},{130,"MEL"},{127,"MEL"},
    {138,"SMG"},{133,"SNI"},{131,"MEL"},
};
static const int WEAPON_COUNT = sizeof(WEAPONS)/sizeof(WEAPONS[0]);

static std::unordered_map<std::string, float> g_nsAnim;
static inline float NS_Anim(const std::string& key, float target, float speed = 14.f) {
    float dt = ImGui::GetIO().DeltaTime; if (dt <= 0.f) dt = 1.f/60.f;
    auto it = g_nsAnim.find(key);
    if (it == g_nsAnim.end()) { g_nsAnim[key] = target; return target; }
    float cur = it->second;
    float t = 1.f - std::exp(-speed * dt);
    cur = cur + (target - cur) * t;
    if (std::fabs(cur - target) < 0.001f) cur = target;
    it->second = cur; return cur;
}
static inline ImU32 NS_Lerp(ImU32 a, ImU32 b, float t) {
    t = std::clamp(t, 0.f, 1.f);
    int ar=(a>>16)&0xFF, ag=(a>>8)&0xFF, ab=a&0xFF, aa=(a>>24)&0xFF;
    int br=(b>>16)&0xFF, bg=(b>>8)&0xFF, bb=b&0xFF, ba=(b>>24)&0xFF;
    return IM_COL32((int)(ar+(br-ar)*t),(int)(ag+(bg-ag)*t),(int)(ab+(bb-ab)*t),(int)(aa+(ba-aa)*t));
}

static void NS_Panel(ImDrawList* dl, ImVec2 a, ImVec2 b, float rounding = 6.f) {
    dl->AddRectFilled(a, b, C_BgCard(), rounding);
    dl->AddRect(a, b, C_Border(), rounding, 0, 0.5f);
}

static bool NS_Toggle(const char* id, const char* label, bool* value) {
    ImGui::PushID(id);
    float avail = ImGui::GetContentRegionAvail().x;
    ImVec2 pos = ImGui::GetCursorScreenPos();
    float h = 34.f, tw = 38.f, th = 18.f;
    ImGui::InvisibleButton("btn", ImVec2(avail, h));
    bool clicked = ImGui::IsItemClicked(); if (clicked) *value = !*value;
    bool hover = ImGui::IsItemHovered();
    ImDrawList* dl = ImGui::GetWindowDrawList();
    if (dl) {
        std::string key = std::string(id) + "#a";
        float on = NS_Anim(key+"on", *value ? 1.f : 0.f, 14.f);
        float hov = NS_Anim(key+"hov", hover ? 1.f : 0.f, 16.f);
        ImU32 bg = NS_Lerp(C_BgCard(), C_BgCardHov(), hov);
        dl->AddRectFilled(pos, ImVec2(pos.x+avail, pos.y+h), bg, 4.f);
        float lh = ImGui::CalcTextSize(label).y;
        dl->AddText(ImGui::GetFont(), 12.f, ImVec2(pos.x+10, pos.y+(h-lh)*0.5f), NS_Lerp(C_TextDim(), C_TextMain(), on), label);
        float tx = pos.x + avail - tw - 10.f, ty = pos.y + (h - th) * 0.5f;
        dl->AddRectFilled(ImVec2(tx, ty), ImVec2(tx+tw, ty+th), NS_Lerp(IM_COL32(45,45,52,255), C_Accent(), on), th*0.5f);
        float knobR = th*0.5f - 1.5f, knobX = tx + 1.5f + (tw - th - 3.f) * on;
        dl->AddCircleFilled(ImVec2(knobX+knobR, ty+th*0.5f), knobR, IM_COL32(240,240,245,255));
    }
    ImGui::PopID(); ImGui::Dummy(ImVec2(0, 3)); return clicked;
}
static bool NS_Toggle(const char* id, const char* label, std::atomic<bool>* value) {
    bool v = value->load(); bool c = NS_Toggle(id, label, &v); if (c) value->store(v); return c;
}

static bool NS_Slider(const char* id, const char* label, float* value, float minV, float maxV, const char* suffix = "") {
    ImGui::PushID(id);
    float avail = ImGui::GetContentRegionAvail().x;
    ImVec2 pos = ImGui::GetCursorScreenPos();
    float h = 42.f;
    ImDrawList* dl = ImGui::GetWindowDrawList();
    bool hoverRow = ImGui::IsMouseHoveringRect(pos, ImVec2(pos.x+avail, pos.y+h));
    if (dl) {
        std::string key = std::string(id) + "#s";
        float hov = NS_Anim(key+"hov", hoverRow ? 1.f : 0.f, 16.f);
        dl->AddText(ImGui::GetFont(), 12.f, ImVec2(pos.x+10, pos.y+4), C_TextDim(), label);
        char valStr[32]; snprintf(valStr, sizeof(valStr), "%.1f%s", *value, suffix);
        float vw = ImGui::CalcTextSize(valStr).x;
        dl->AddText(ImGui::GetFont(), 11.f, ImVec2(pos.x+avail-vw-8, pos.y+4), C_TextDim(), valStr);
        float ty = pos.y + 26.f, tw2 = avail - 20.f;
        float rawPct = (maxV > minV) ? std::clamp((*value-minV)/(maxV-minV),0.f,1.f) : 0.f;
        float pct = NS_Anim(key+"pct", rawPct, 20.f);
        dl->AddRectFilled(ImVec2(pos.x+10,ty), ImVec2(pos.x+10+tw2,ty+3), IM_COL32(40,40,48,255), 2.f);
        dl->AddRectFilled(ImVec2(pos.x+10,ty), ImVec2(pos.x+10+tw2*pct,ty+3), C_Accent(), 2.f);
        dl->AddCircleFilled(ImVec2(pos.x+10+tw2*pct, ty+1.5f), 5.f, IM_COL32(240,240,245,255));
        ImGui::SetCursorScreenPos(ImVec2(pos.x+4, ty-8)); ImGui::SetNextItemWidth(tw2+12);
    }
    bool changed = ImGui::SliderFloat("##s", value, minV, maxV, "");
    ImGui::Dummy(ImVec2(0, 4)); ImGui::PopID(); return changed;
}

static void NS_Header(const char* title) {
    ImDrawList* dl = ImGui::GetWindowDrawList();
    ImVec2 pos = ImGui::GetCursorScreenPos();
    if (dl) dl->AddText(ImGui::GetFont(), 13.f, ImVec2(pos.x, pos.y+2), C_TextMain(), title);
    ImGui::Dummy(ImVec2(0, 20));
}

static bool NS_Button(const char* id, const char* label, ImVec2 size, bool accent = true) {
    ImGui::PushID(id);
    ImVec2 pos = ImGui::GetCursorScreenPos();
    bool hover = ImGui::IsMouseHoveringRect(pos, ImVec2(pos.x+size.x, pos.y+size.y));
    bool down  = hover && ImGui::IsMouseDown(ImGuiMouseButton_Left);
    ImDrawList* dl = ImGui::GetWindowDrawList();
    if (dl) {
        std::string key = std::string(id)+"#ba";
        float hov = NS_Anim(key+"hov", hover?1.f:0.f, 18.f);
        float prs = NS_Anim(key+"prs", down?1.f:0.f, 24.f);
        ImVec2 a = pos, b = ImVec2(pos.x+size.x, pos.y+size.y);
        float inset = prs*1.f; a.x+=inset; a.y+=inset; b.x-=inset; b.y-=inset;
        ImU32 bg = accent ? NS_Lerp(C_Accent(), C_AccentBright(), hov) : NS_Lerp(C_BgCard(), C_BgCardHov(), hov);
        dl->AddRectFilled(a, b, bg, 4.f);
        float lw = ImGui::CalcTextSize(label).x;
        dl->AddText(ImVec2(pos.x+(size.x-lw)*0.5f, pos.y+size.y*0.5f-7), accent ? IM_COL32(255,255,255,255) : C_TextMain(), label);
    }
    ImGui::SetCursorScreenPos(pos); ImGui::InvisibleButton("btn", size);
    bool clicked = ImGui::IsItemClicked(); ImGui::Dummy(size); ImGui::PopID(); return clicked;
}

static void NS_InfoRow(const char* label, const char* value, ImU32 valueColor = 0) {
    ImDrawList* dl = ImGui::GetWindowDrawList();
    ImVec2 pos = ImGui::GetCursorScreenPos();
    float avail = ImGui::GetContentRegionAvail().x;
    float rh = 34.f;
    if (dl) {
        bool hov = ImGui::IsMouseHoveringRect(pos, ImVec2(pos.x+avail, pos.y+rh));
        float ha = NS_Anim(std::string(label)+"#irh", hov?1.f:0.f, 16.f);
        ImU32 bg = NS_Lerp(C_BgCard(), C_BgCardHov(), ha);
        dl->AddRectFilled(pos, ImVec2(pos.x+avail, pos.y+rh), bg, 4.f);
        dl->AddRect(pos, ImVec2(pos.x+avail, pos.y+rh), C_Border(), 4.f, 0, 0.5f);
        dl->AddText(ImGui::GetFont(), 12.f, ImVec2(pos.x+10, pos.y+10), C_TextDim(), label);
        float vw = ImGui::CalcTextSize(value).x;
        ImU32 fc = valueColor ? valueColor : C_TextMain();
        dl->AddText(ImGui::GetFont(), 12.f, ImVec2(pos.x+avail-vw-10, pos.y+10), fc, value);
    }
    ImGui::Dummy(ImVec2(0, rh+4));
}

static bool NS_ComboRow(const char* id, const char* label, int* value, const char** items, int itemCount) {
    ImGui::PushID(id);
    float avail = ImGui::GetContentRegionAvail().x;
    ImVec2 pos = ImGui::GetCursorScreenPos();
    float h = 34.f;
    NS_Panel(ImGui::GetWindowDrawList(), pos, ImVec2(pos.x+avail, pos.y+h));
    int cur = *value;
    const char* curText = (cur>=0 && cur<itemCount) ? items[cur] : "?";
    ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), 12.f, ImVec2(pos.x+10, pos.y+10), C_TextDim(), label);
    ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), 12.f, ImVec2(pos.x+avail-ImGui::CalcTextSize(curText).x-10, pos.y+10), C_TextMain(), curText);
    ImGui::SetCursorScreenPos(ImVec2(pos.x+4, pos.y+4));
    ImGui::SetNextItemWidth(avail-8);
    bool changed = ImGui::Combo("##combo", value, items, itemCount);
    ImGui::PopID();
    ImGui::Dummy(ImVec2(0, 4));
    return changed;
}

static std::string GetSkinNameWithLoading(int wid, int skinIdx) {
    char nameBuf[128];
    if (SkinInfoSync::GetSkinName(wid, skinIdx, nameBuf, sizeof(nameBuf))) return std::string(nameBuf);
    return "Loading...";
}
static std::string GetWeaponNameWithLoading(int wid) {
    char nameBuf[128];
    if (SkinInfoSync::GetWeaponName(wid, nameBuf, sizeof(nameBuf))) return std::string(nameBuf);
    return "Loading...";
}
static std::string GetHeroNameWithLoading(int heroId) {
    char nameBuf[128];
    if (SkinInfoSync::GetHeroName(heroId, nameBuf, sizeof(nameBuf))) return std::string(nameBuf);
    return "Loading...";
}
static std::string GetHeroSkinNameWithLoading(int heroId, int skinIdx) {
    char nameBuf[128];
    if (SkinInfoSync::GetHeroSkinName(heroId, skinIdx, nameBuf, sizeof(nameBuf))) return std::string(nameBuf);
    return "Loading...";
}
static int GetHeroSkinLevelWithLoading(int heroId, int skinIdx) {
    return SkinInfoSync::GetHeroSkinLevel(heroId, skinIdx);
}

static bool NS_SkinList(const char* id, int* cur, int max, float w, int wid) {
    if (max<=0) return false;
    if (*cur<0) *cur=0; if (*cur>max) *cur=max;
    bool changed=false;
    const float rowH = 40.f, gap = 3.f;
    float listH = max * rowH + (max>0?(max-1):0) * gap + 6.f;
    listH = std::clamp(listH, 50.f, 280.f);
    ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarSize, 12.f);
    ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarRounding, 6.f);
    ImGui::BeginChild(id, ImVec2(w, listH), false, ImGuiWindowFlags_AlwaysVerticalScrollbar);
    ImDrawList* gdl = ImGui::GetWindowDrawList();
    for (int s = 1; s <= max; s++) {
        std::string skinName = GetSkinNameWithLoading(wid, s);
        bool act = (*cur == s);
        ImVec2 p = ImGui::GetCursorScreenPos();
        bool hov = ImGui::IsMouseHoveringRect(p, ImVec2(p.x+w, p.y+rowH));
        if (gdl) {
            ImU32 bg = act ? C_Accent() : hov ? C_BgCardHov() : C_BgCard();
            gdl->AddRectFilled(p, ImVec2(p.x+w, p.y+rowH), bg, 4.f);
            float th = ImGui::GetTextLineHeight();
            float nameX = p.x + 10.f, maxTextW = w - 20.f;
            std::string disp = skinName;
            ImVec2 tsize = ImGui::CalcTextSize(disp.c_str());
            while (tsize.x > maxTextW && disp.size() > 1) { disp.pop_back(); tsize = ImGui::CalcTextSize((disp+"...").c_str()); }
            if (disp.size() < skinName.size()) disp += "...";
            ImU32 tc = (act||hov) ? IM_COL32(255,255,255,255) : C_TextSec();
            if (skinName == "Loading...") tc = C_TextDim();
            gdl->AddText(ImGui::GetFont(), 12.f, ImVec2(nameX, p.y+(rowH-th)*0.5f), tc, disp.c_str());
            if (act) gdl->AddRectFilled(ImVec2(p.x+w-16, p.y+6), ImVec2(p.x+w-6, p.y+rowH-6), IM_COL32(0,255,100,120), 3.f);
        }
        ImGui::PushID(s + 90000);
        ImGui::InvisibleButton("skrow", ImVec2(w, rowH));
        if (ImGui::IsItemClicked()) { *cur = s; changed = true; }
        ImGui::PopID();
        if (s < max) ImGui::Dummy(ImVec2(0, gap));
    }
    ImGui::EndChild();
    ImGui::PopStyleVar(2);
    return changed;
}

static bool NS_HeroSkinList(const char* id, int* cur, int max, float w, int heroId) {
    if (max<=0) return false;
    if (*cur<0) *cur=0; if (*cur>max) *cur=max;
    bool changed=false;
    const float rowH = 40.f, gap = 3.f;
    float listH = max * rowH + (max>0?(max-1):0) * gap + 6.f;
    listH = std::clamp(listH, 50.f, 280.f);
    ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarSize, 12.f);
    ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarRounding, 6.f);
    ImGui::BeginChild(id, ImVec2(w, listH), false, ImGuiWindowFlags_AlwaysVerticalScrollbar);
    ImDrawList* gdl = ImGui::GetWindowDrawList();
    for (int s = 1; s <= max; s++) {
        std::string skinName = GetHeroSkinNameWithLoading(heroId, s);
        int level = GetHeroSkinLevelWithLoading(heroId, s);
        bool act = (*cur == s);
        ImVec2 p = ImGui::GetCursorScreenPos();
        bool hov = ImGui::IsMouseHoveringRect(p, ImVec2(p.x+w, p.y+rowH));
        if (gdl) {
            ImU32 bg = act ? C_Accent() : hov ? C_BgCardHov() : C_BgCard();
            gdl->AddRectFilled(p, ImVec2(p.x+w, p.y+rowH), bg, 4.f);
            float th = ImGui::GetTextLineHeight();
            float nameX = p.x + 10.f, maxTextW = w - 70.f;
            std::string disp = skinName;
            ImVec2 tsize = ImGui::CalcTextSize(disp.c_str());
            while (tsize.x > maxTextW && disp.size() > 1) { disp.pop_back(); tsize = ImGui::CalcTextSize((disp+"...").c_str()); }
            if (disp.size() < skinName.size()) disp += "...";
            ImU32 tc = (act||hov) ? IM_COL32(255,255,255,255) : C_TextSec();
            if (skinName == "Loading...") tc = C_TextDim();
            gdl->AddText(ImGui::GetFont(), 12.f, ImVec2(nameX, p.y+(rowH-th)*0.5f), tc, disp.c_str());
            
            if (level > 0) {
                char levelStr[16];
                snprintf(levelStr, sizeof(levelStr), "LV%d", level);
                float lw2 = ImGui::CalcTextSize(levelStr).x;
                float lx = p.x + w - lw2 - 12.f;
                ImU32 lc = act ? IM_COL32(255,255,255,255) : C_Warning();
                gdl->AddText(ImGui::GetFont(), 10.f, ImVec2(lx, p.y+(rowH-ImGui::GetTextLineHeight())*0.5f), lc, levelStr);
            }
            
            if (act) gdl->AddRectFilled(ImVec2(p.x+w-16, p.y+6), ImVec2(p.x+w-6, p.y+rowH-6), IM_COL32(0,255,100,120), 3.f);
        }
        ImGui::PushID(s + 90000);
        ImGui::InvisibleButton("skrow", ImVec2(w, rowH));
        if (ImGui::IsItemClicked()) { *cur = s; changed = true; }
        ImGui::PopID();
        if (s < max) ImGui::Dummy(ImVec2(0, gap));
    }
    ImGui::EndChild();
    ImGui::PopStyleVar(2);
    return changed;
}

static void NS_WeaponSkinSection(int selWep, float w, bool& needSync) {
    if (selWep<0||selWep>=WEAPON_COUNT) return;
    if (!g_cjkLoaded) g_pending_cjk_reload = true;
    int wid = WEAPONS[selWep].id; if (wid==0) wid = selWep+1;
    std::string weaponName = GetWeaponNameWithLoading(wid);
    int maxS = SkinInfoSync::GetSkinCount(wid);
    char key[32]; snprintf(key,sizeof(key),"iSkin_%d",wid);
    if (!sync_int.count(key)) sync_int[key]=0;
    int cur = sync_int[key];
    ImDrawList* sdl = ImGui::GetWindowDrawList();
    {
        ImVec2 hp = ImGui::GetCursorScreenPos();
        NS_Panel(sdl, hp, ImVec2(hp.x+w, hp.y+44));
        bool isLoading = (weaponName == "Loading...");
        sdl->AddText(ImGui::GetFont(), 14.f, ImVec2(hp.x+10, hp.y+8), isLoading ? C_TextDim() : C_TextMain(), weaponName.c_str());
        if (cur > 0) {
            std::string skinName = GetSkinNameWithLoading(wid, cur);
            sdl->AddText(ImGui::GetFont(), 11.f, ImVec2(hp.x+10, hp.y+26), (skinName == "Loading...") ? C_TextDim() : C_Success(), skinName.c_str());
        } else {
            sdl->AddText(ImGui::GetFont(), 11.f, ImVec2(hp.x+10, hp.y+26), C_TextDim(), "Default Skin");
        }
        ImGui::Dummy(ImVec2(0, 52));
    }
    if (NS_Button("def_skin", "↺ Default", ImVec2(w, 30), false)) {
        cur=0; sync_int[key]=0; needSync=true;
        AddNotification("Weapon skin reset to default", 200, 200, 100, 1.5f);
    }
    ImGui::Dummy(ImVec2(0, 6));
    if (maxS > 0) {
        bool skinChanged = NS_SkinList("wg", &cur, maxS, w, wid);
        if (skinChanged) {
            sync_int[key]=cur; needSync=true;
            std::string skinName = GetSkinNameWithLoading(wid, cur);
            char msg[96];
            if (skinName != "Loading...") snprintf(msg, sizeof(msg), "Skin: %s", skinName.c_str());
            else snprintf(msg, sizeof(msg), "Skin #%d selected", cur);
            AddNotification(msg, 100, 200, 255, 1.5f);
        }
    } else {
        ImVec2 p = ImGui::GetCursorScreenPos();
        NS_Panel(sdl, p, ImVec2(p.x+w, p.y+42));
        sdl->AddText(ImGui::GetFont(), 12.f, ImVec2(p.x+10, p.y+12), C_TextDim(), "No skins available for this weapon");
        ImGui::Dummy(ImVec2(0, 50));
    }
}

static void NS_WeaponList(int activeCat, int& selWep, float w) {
    const char* cm[]={"AR","SMG","SNI","SHT","PST","MEL","LMG"};
    const char* cat=cm[activeCat];
    ImGui::BeginChild("weapon_list", ImVec2(w, 340), false);
    ImDrawList* dl = ImGui::GetWindowDrawList();
    for (int i=0;i<WEAPON_COUNT;i++) {
        if (strcmp(WEAPONS[i].cat,cat)!=0) continue;
        bool sel=(selWep==i);
        ImVec2 p=ImGui::GetCursorScreenPos();
        float aw=ImGui::GetContentRegionAvail().x;
        float rowH = 38.f;
        if (dl) {
            bool hov=ImGui::IsMouseHoveringRect(p,ImVec2(p.x+aw,p.y+rowH));
            ImU32 bg = sel ? C_Accent() : (hov ? C_BgCardHov() : C_BgCard());
            dl->AddRectFilled(p, ImVec2(p.x+aw,p.y+rowH), bg, 4.f);
            if (sel) dl->AddRectFilled(ImVec2(p.x,p.y+4),ImVec2(p.x+2,p.y+rowH-4), C_AccentBright(), 1.f);
            std::string weaponName = GetWeaponNameWithLoading(WEAPONS[i].id);
            dl->AddText(ImGui::GetFont(), 12.f, ImVec2(p.x+12, p.y+(rowH-ImGui::GetTextLineHeight())*0.5f),
                sel ? IM_COL32(255,255,255,255) : (hov ? C_TextMain() : C_TextSec()), weaponName.c_str());
        }
        ImGui::PushID(i); ImGui::InvisibleButton("w",ImVec2(aw,rowH));
        if (ImGui::IsItemClicked()) selWep=i;
        ImGui::PopID(); ImGui::Dummy(ImVec2(0,2));
    }
    ImGui::EndChild();
}

static void NS_WeaponCategories(int& activeCat, int& selWep) {
    const char* cats[]={"AR","SMG","Sniper Rifles","Shotguns","Pistols","Melee","LMG"};
    float av=ImGui::GetContentRegionAvail().x;
    float bw=(av-(6*4.f))/7.f; if (bw<45) bw=45;
    for (int i=0;i<7;i++) {
        if (i>0) ImGui::SameLine(0,4);
        bool act=(activeCat==i);
        ImGui::PushStyleColor(ImGuiCol_Button, act ? C_Accent() : C_BgCard());
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, act ? C_AccentBright() : C_BgCardHov());
        ImGui::PushStyleColor(ImGuiCol_Text, act ? IM_COL32(255,255,255,255) : C_TextSec());
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4.f);
        ImGui::PushID(i);
        if (ImGui::Button(cats[i], ImVec2(bw,28))) {
            activeCat=i;
            const char* tc=(i==0?"AR":i==1?"SMG":i==2?"SNI":i==3?"SHT":i==4?"PST":i==5?"MEL":"LMG");
            for (int w=0;w<WEAPON_COUNT;w++) if (strcmp(WEAPONS[w].cat,tc)==0){selWep=w;break;}
        }
        ImGui::PopID(); ImGui::PopStyleVar(); ImGui::PopStyleColor(3);
    }
}

static void NS_HeroSkinSection(float w, bool& needSync) {
    ImDrawList* hdl = ImGui::GetWindowDrawList();
    needSync |= NS_Toggle("hero_en", "Hero Skin Hack", &sync_bool["bHeroSkinHack"]);
    
    if (!sync_bool["bHeroSkinHack"]) {
        ImVec2 p = ImGui::GetCursorScreenPos();
        NS_Panel(hdl, p, ImVec2(p.x+w, p.y+42));
        hdl->AddText(ImGui::GetFont(), 12.f, ImVec2(p.x+10, p.y+12), C_TextDim(), "Enable Hero Skin Hack first");
        ImGui::Dummy(ImVec2(0, 50));
        return;
    }
    
    std::vector<int> heroIds = SkinInfoSync::GetAllHeroIds();
    if (heroIds.empty()) {
        ImVec2 p = ImGui::GetCursorScreenPos();
        NS_Panel(hdl, p, ImVec2(p.x+w, p.y+42));
        hdl->AddText(ImGui::GetFont(), 12.f, ImVec2(p.x+10, p.y+12), C_TextDim(), "Loading...");
        ImGui::Dummy(ImVec2(0, 50));
        return;
    }
    
    static int selHeroIdx = 0;
    if (selHeroIdx >= (int)heroIds.size()) selHeroIdx = 0;
    float lw = (w - 12) * 0.35f, rw = (w - 12) * 0.62f;

    ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarSize, 12.f);
    ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarRounding, 6.f);
    ImGui::BeginChild("hero_list_container", ImVec2(lw, 340), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);
    ImDrawList* hldl = ImGui::GetWindowDrawList();
    for (int i = 0; i < (int)heroIds.size(); i++) {
        int hid = heroIds[i]; bool sel = (selHeroIdx == i);
        ImVec2 p = ImGui::GetCursorScreenPos(); float aw = ImGui::GetContentRegionAvail().x; float rowH = 38.f;
        std::string heroName = GetHeroNameWithLoading(hid);
        if (hldl) {
            bool hov = ImGui::IsMouseHoveringRect(p, ImVec2(p.x+aw, p.y+rowH));
            ImU32 bg = sel ? C_Accent() : (hov ? C_BgCardHov() : C_BgCard());
            hldl->AddRectFilled(p, ImVec2(p.x+aw, p.y+rowH), bg, 4.f);
            if (sel) hldl->AddRectFilled(ImVec2(p.x,p.y+4),ImVec2(p.x+2,p.y+rowH-4), C_AccentBright(), 1.f);
            hldl->AddText(ImGui::GetFont(), 12.f, ImVec2(p.x+12, p.y+(rowH-ImGui::GetTextLineHeight())*0.5f),
                sel ? IM_COL32(255,255,255,255) : (hov ? C_TextMain() : C_TextSec()), heroName.c_str());
        }
        ImGui::PushID(i + 1000); ImGui::InvisibleButton("h", ImVec2(aw, rowH));
        if (ImGui::IsItemClicked()) selHeroIdx = i;
        ImGui::PopID(); ImGui::Dummy(ImVec2(0, 2));
    }
    ImGui::EndChild();
    ImGui::PopStyleVar(2);
    ImGui::SameLine(0, 10);

    ImGui::BeginChild("hero_skin_grid_container", ImVec2(rw, 340), false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    int hid = heroIds[selHeroIdx];
    int maxS = SkinInfoSync::GetHeroSkinCount(hid);
    std::string heroName = GetHeroNameWithLoading(hid);
    char key[32]; snprintf(key, sizeof(key), "iHeroSkin_%d", hid);
    if (!sync_int.count(key)) sync_int[key] = 0;
    int cur = sync_int[key];
    ImDrawList* sdl = ImGui::GetWindowDrawList();
    {
        ImVec2 hp = ImGui::GetCursorScreenPos();
        NS_Panel(sdl, hp, ImVec2(hp.x+rw, hp.y+44));
        sdl->AddText(ImGui::GetFont(), 14.f, ImVec2(hp.x+10, hp.y+8), C_TextMain(), heroName.c_str());
        if (cur > 0) {
            std::string skinName = GetHeroSkinNameWithLoading(hid, cur);
            int level = GetHeroSkinLevelWithLoading(hid, cur);
            char display[160];
            if (level > 0) snprintf(display, sizeof(display), "%s  [LV%d]", skinName.c_str(), level);
            else snprintf(display, sizeof(display), "%s", skinName.c_str());
            sdl->AddText(ImGui::GetFont(), 11.f, ImVec2(hp.x+10, hp.y+26), C_Success(), display);
        } else {
            sdl->AddText(ImGui::GetFont(), 11.f, ImVec2(hp.x+10, hp.y+26), C_TextDim(), "Default Skin");
        }
        ImGui::Dummy(ImVec2(0, 52));
    }
    if (NS_Button("hero_def", "↺ Default", ImVec2(rw, 30), false)) {
        cur = 0; sync_int[key] = 0; needSync = true;
        AddNotification("Hero skin reset to default", 200, 200, 100, 1.5f);
    }
    ImGui::Dummy(ImVec2(0, 6));
    if (maxS > 0) {
        bool skinChanged = NS_HeroSkinList("hg_list", &cur, maxS, rw, hid);
        if (skinChanged) {
            sync_int[key] = cur; needSync = true;
            std::string skinName = GetHeroSkinNameWithLoading(hid, cur);
            int level = GetHeroSkinLevelWithLoading(hid, cur);
            char msg[160];
            if (level > 0) snprintf(msg, sizeof(msg), "%s [LV%d]", skinName.c_str(), level);
            else snprintf(msg, sizeof(msg), "%s", skinName.c_str());
            AddNotification(msg, 100, 200, 255, 1.5f);
        }
    } else {
        ImVec2 p = ImGui::GetCursorScreenPos();
        NS_Panel(sdl, p, ImVec2(p.x+rw, p.y+42));
        sdl->AddText(ImGui::GetFont(), 12.f, ImVec2(p.x+10, p.y+12), C_TextDim(), "No skins available");
        ImGui::Dummy(ImVec2(0, 50));
    }
    ImGui::EndChild();
}

void DrawMenu(ImGuiIO& io) {
    ImGui::SetNextWindowSize(ImVec2(860, 560), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSizeConstraints(ImVec2(660, 460), ImVec2(1400, 900));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 8.f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0,0));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, C_BgDark());
    ImGui::PushStyleColor(ImGuiCol_Border, C_Border());
    ImGui::PushStyleColor(ImGuiCol_BorderShadow, IM_COL32(0,0,0,0));
    ImGui::PushStyleColor(ImGuiCol_ScrollbarBg, C_BgCard());
    ImGui::PushStyleColor(ImGuiCol_ScrollbarGrab, C_BorderLight());
    ImGui::PushStyleColor(ImGuiCol_FrameBg, C_BgInput());
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, C_BgCardHov());
    ImGui::PushStyleColor(ImGuiCol_Button, C_BgCard());
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, C_BgCardHov());
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, C_Accent());
    ImGui::PushStyleColor(ImGuiCol_Text, C_TextMain());
    ImGui::PushStyleColor(ImGuiCol_TextDisabled, C_TextDim());
    ImGui::PushStyleColor(ImGuiCol_TitleBg, C_BgSidebar());
    ImGui::PushStyleColor(ImGuiCol_ChildBg, C_BgCard());

    ImGui::Begin("##NexoraMenu",nullptr, ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoScrollbar|ImGuiWindowFlags_NoCollapse);

    ImVec2 wp = ImGui::GetWindowPos();
    ImVec2 ws = ImGui::GetWindowSize();
    ImDrawList* dl = ImGui::GetWindowDrawList();

    dl->AddRectFilled(wp, ImVec2(wp.x+ws.x, wp.y+ws.y), C_BgDark(), 8.f);
    float sbW = 155.f;
    dl->AddRectFilled(ImVec2(wp.x, wp.y), ImVec2(wp.x+sbW, wp.y+ws.y), C_BgSidebar(), 8.f, ImDrawFlags_RoundCornersLeft);
    dl->AddLine(ImVec2(wp.x+sbW, wp.y), ImVec2(wp.x+sbW, wp.y+ws.y), C_Border(), 1.f);

    if (g_logo_tex) {
        dl->AddImageRounded((ImTextureID)(intptr_t)g_logo_tex,
            ImVec2(wp.x+10, wp.y+10), ImVec2(wp.x+46, wp.y+46),
            ImVec2(0,0),ImVec2(1,1),IM_COL32(255,255,255,255),8.f);
    } else {
        dl->AddCircleFilled(ImVec2(wp.x+28,wp.y+28),16,C_Accent());
        dl->AddText(ImGui::GetFont(),16.f,ImVec2(wp.x+22,wp.y+22),IM_COL32(255,255,255,255),"N");
    }
    dl->AddText(ImGui::GetFont(), 13.f, ImVec2(wp.x+54, wp.y+18), C_TextMain(), "Nexora");
    dl->AddText(ImGui::GetFont(), 9.f, ImVec2(wp.x+54, wp.y+32), C_TextDim(), "Blood Strike");

    static int activeTab = 0;
    const char* navLabels[] = {
        "Dashboard",
        "Aimbot",
        "Visuals",
        "Skins",
        "Hero Skins",
        "Protection",
        "Settings",
        "Memory"
    };
    float navStartY = wp.y + 56.f, navItemH = 34.f;
    for (int i = 0; i < 8; i++) {
        float ny = navStartY + i * navItemH;
        bool act = (activeTab == i);
        bool hov = ImGui::IsMouseHoveringRect(ImVec2(wp.x+4, ny+2), ImVec2(wp.x+sbW-4, ny+navItemH-2));
        if (act) dl->AddRectFilled(ImVec2(wp.x+4, ny+2), ImVec2(wp.x+sbW-4, ny+navItemH-2), C_Accent(), 4.f);
        else if (hov) dl->AddRectFilled(ImVec2(wp.x+4, ny+2), ImVec2(wp.x+sbW-4, ny+navItemH-2), C_BgCardHov(), 4.f);
        ImU32 textCol = act ? IM_COL32(255,255,255,255) : (hov ? C_TextMain() : C_TextDim());
        dl->AddText(ImGui::GetFont(), 12.f, ImVec2(wp.x+12, ny+10), textCol, navLabels[i]);
        ImGui::SetCursorScreenPos(ImVec2(wp.x+4, ny+2));
        ImGui::PushID(i+500);
        if (ImGui::InvisibleButton("nav", ImVec2(sbW-8, navItemH-4))) activeTab = i;
        ImGui::PopID();
    }

    float footerY = wp.y + ws.y - 45.f;
    dl->AddLine(ImVec2(wp.x+8, footerY), ImVec2(wp.x+sbW-8, footerY), C_Border(), 0.5f);
    dl->AddText(ImGui::GetFont(), 11.f, ImVec2(wp.x+10, footerY+6), C_TextDim(), APP_MAKER);
    dl->AddText(ImGui::GetFont(), 10.f, ImVec2(wp.x+10, footerY+20), C_Success(), APP_VERSION);

    float tbX = wp.x + sbW, tbY = wp.y, tbW = ws.x - sbW, tbH = 48.f;
    dl->AddRectFilled(ImVec2(tbX, tbY), ImVec2(tbX+tbW, tbY+tbH), C_BgSidebar(), 8.f, ImDrawFlags_RoundCornersTopRight);
    dl->AddLine(ImVec2(tbX, tbY+tbH), ImVec2(tbX+tbW, tbY+tbH), C_Border(), 1.f);

    float saveX = tbX + 120.f, saveY = tbY + 10.f, saveW = 65.f, saveH = 26.f;
    bool saveHov = ImGui::IsMouseHoveringRect(ImVec2(saveX, saveY), ImVec2(saveX+saveW, saveY+saveH));
    ImU32 saveBg = NS_Lerp(C_Accent(), C_AccentBright(), saveHov?1.f:0.f);
    dl->AddRectFilled(ImVec2(saveX, saveY), ImVec2(saveX+saveW, saveY+saveH), saveBg, 4.f);
    float saveLw = ImGui::CalcTextSize("Save").x;
    dl->AddText(ImVec2(saveX+(saveW-saveLw)*0.5f, saveY+6), IM_COL32(255,255,255,255), "Save");
    ImGui::SetCursorScreenPos(ImVec2(saveX, saveY));
    ImGui::PushID(800);
    if (ImGui::InvisibleButton("save", ImVec2(saveW, saveH))) { SaveConfig(); AddNotification("Saved", 80, 220, 80, 1.5f); }
    ImGui::PopID();

    time_t now = time(nullptr); struct tm* tm_info = localtime(&now); char clockBuf[16]; strftime(clockBuf, sizeof(clockBuf), "%H:%M", tm_info);
    dl->AddText(ImGui::GetFont(), 12.f, ImVec2(tbX+tbW-50, tbY+16), C_TextDim(), clockBuf);

    float closeX = tbX+tbW-34, closeY = tbY+6;
    bool hoverClose = ImGui::IsMouseHoveringRect(ImVec2(closeX, closeY), ImVec2(closeX+28, closeY+28));
    ImU32 closeBg = NS_Lerp(C_BgCard(), C_Danger(), hoverClose?1.f:0.f);
    dl->AddCircleFilled(ImVec2(closeX+14, closeY+14), 12, closeBg);
    ImVec2 c = ImVec2(closeX+14, closeY+14); float r = 4.f;
    dl->AddLine(ImVec2(c.x-r,c.y-r), ImVec2(c.x+r,c.y+r), IM_COL32(255,255,255,220), 1.5f);
    dl->AddLine(ImVec2(c.x-r,c.y+r), ImVec2(c.x+r,c.y-r), IM_COL32(255,255,255,220), 1.5f);
    ImGui::SetCursorScreenPos(ImVec2(closeX, closeY));
    ImGui::PushID(9999);
    if (ImGui::InvisibleButton("hide", ImVec2(28, 28))) menu_visible = false;
    ImGui::PopID();

    float contentX = tbX + 12.f, contentY = tbY + tbH + 10.f;
    float contentW = tbW - 24.f, contentH = ws.y - contentY + wp.y - 12.f;
    ImGui::SetCursorScreenPos(ImVec2(contentX, contentY));
    ImGui::BeginChild("content", ImVec2(contentW, contentH), false, ImGuiWindowFlags_NoBackground);
    dl = ImGui::GetWindowDrawList();
    float cw = ImGui::GetContentRegionAvail().x;
    bool needSync = false;
    float colW = (cw - 10.f) * 0.5f;

    if (activeTab == 0) {
        NS_Header("Live Stats");
        {
            ImVec2 fp = ImGui::GetCursorScreenPos();
            NS_Panel(dl, fp, ImVec2(fp.x+cw, fp.y+50));
            char fpsT[16]; snprintf(fpsT, sizeof(fpsT), "%.0f", io.Framerate);
            ImU32 fc = io.Framerate > 55 ? C_Success() : (io.Framerate > 30 ? C_Warning() : C_Danger());
            dl->AddText(ImGui::GetFont(), 24.f, ImVec2(fp.x+12, fp.y+10), fc, fpsT);
            dl->AddText(ImGui::GetFont(), 11.f, ImVec2(fp.x+12+ImGui::CalcTextSize(fpsT).x+6, fp.y+20), C_TextDim(), "FPS");
            ImGui::Dummy(ImVec2(0, 58));
        }
        ImGui::Dummy(ImVec2(0, 12));
        NS_Header("About");
        NS_InfoRow("Version", APP_VERSION);
        NS_InfoRow("Developer", APP_MAKER);
        NS_InfoRow("Telegram", APP_CHANNEL);
        NS_InfoRow("Game", "Blood Strike");
    }

    else if (activeTab == 1) {
        NS_Header("Aimbot Configuration");
        // Online feature gating removed: every feature is always available.
        {
            ImGui::Columns(2, "aim_cols", false);
            ImGui::SetColumnWidth(0, colW);
            needSync |= NS_Toggle("aim_en", "Master Switch", &sync_bool["bAIM"]);
            needSync |= NS_Toggle("aim_vis", "Check Visibility", &sync_bool["bAIM_CheckVisibility"]);
            needSync |= NS_Toggle("aim_knock", "Ignore Knocked", &sync_bool["bAIM_IgnoreKnocked"]);
            needSync |= NS_Toggle("aim_bots", "Ignore Bots", &sync_bool["bAIM_IgnoreBots"]);
            ImGui::NextColumn();
            needSync |= NS_Toggle("aim_pred", "Prediction", &sync_bool["bAIM_Prediction"]);
            needSync |= NS_Toggle("aim_tline", "Target Lines", &sync_bool["bAIM_Lane"]);
            needSync |= NS_Toggle("autotune", "AI Auto Tune", &sync_bool["bAIM_AutoTune"]);
            ImGui::Columns(1);
        }
        ImGui::Dummy(ImVec2(0, 8));
        NS_Header("Parameters");
        ImGui::Columns(2, "aim_params", false);
        ImGui::SetColumnWidth(0, colW);
        needSync |= NS_Slider("aim_fov", "FOV Radius", &sync_float["fAIM_Fov"], 30, 400, "°");
        needSync |= NS_Slider("aim_dist", "Max Distance", &sync_float["fAIM_MaxDist"], 50, 600, "m");
        ImGui::NextColumn();
        needSync |= NS_Slider("aim_snap", "Snap Strength", &sync_float["fAIM_SnapStrength"], 0.5f, 12.f, "x");
        needSync |= NS_Slider("aim_bspd", "Bullet Speed", &sync_float["fAIM_BulletSpeed"], 200, 1200, "m/s");
        ImGui::Columns(1);
        ImGui::Dummy(ImVec2(0, 8));
        NS_Header("Trigger");
        {
            const char* to[]={"OFF", "On Shoot", "On ADS", "Both"};
            needSync |= NS_ComboRow("trigger", "Trigger", &sync_int["iAIM_Trigger"], to, 4);
        }
        ImGui::Dummy(ImVec2(0, 8));
        NS_Header("Target Bone");
        {
            const char* bo[]={"Head", "Neck", "Chest", "Spine", "Pelvis"};
            needSync |= NS_ComboRow("bone", "Target Bone", &sync_int["iAIM_TargetBone"], bo, 5);
        }
    }

    else if (activeTab == 2) {
        NS_Header("Visuals Config");
        // Online feature gating removed: every feature is always available.
        {
            ImGui::Columns(2, "esp_cols", false);
            ImGui::SetColumnWidth(0, colW);
            needSync |= NS_Toggle("esp_en", "Master Switch", &sync_bool["bESP"]);
            needSync |= NS_Toggle("esp_box", "Box ESP", &sync_bool["bESP_Box"]);
            needSync |= NS_Toggle("esp_skel", "Skeleton", &sync_bool["bESP_Skeleton"]);
            needSync |= NS_Toggle("esp_hp", "Health Bar", &sync_bool["bESP_Health"]);
            needSync |= NS_Toggle("esp_armor", "Armor Bar", &sync_bool["bESP_Armor"]);
            ImGui::NextColumn();
            needSync |= NS_Toggle("esp_wep", "Weapon Name", &sync_bool["bESP_Weapon"]);
            needSync |= NS_Toggle("esp_name", "Name Label", &sync_bool["bESP_Name"]);
            needSync |= NS_Toggle("esp_dist", "Distance", &sync_bool["bESP_Distance"]);
            needSync |= NS_Toggle("esp_lane", "Traceline", &sync_bool["bESP_Lane"]);
            ImGui::Columns(1);
            ImGui::Dummy(ImVec2(0, 8));
            needSync |= NS_Toggle("loot_esp_en", "Loot ESP", &sync_bool["bLootESP"]);
            if (sync_bool["bLootESP"]) {
                needSync |= NS_Slider("loot_esp_dist", "Max Distance", &sync_float["fLootESP_MaxDist"], 20, 400, "m");
            }
        }
    }

    else if (activeTab == 3) {
        NS_Header("Skins Config");
        // Online feature gating removed: every feature is always available.
        {
            needSync |= NS_Toggle("skin_en", "Weapon Skin Hack", &sync_bool["bSkinHack"]);
            needSync |= NS_Toggle("unlock_all", "Unlock All Skins", &sync_bool["bUnlockAllSkins"]);
            if (sync_bool["bSkinHack"]) {
                ImGui::Dummy(ImVec2(0, 8));
                static int cat=0, sel=0;
                NS_Header("Select Weapon Type");
                NS_WeaponCategories(cat,sel);
                ImGui::Dummy(ImVec2(0, 8));
                ImGui::Columns(2, "wep_cols", false);
                ImGui::SetColumnWidth(0, colW);
                ImGui::BeginChild("wep_list_container", ImVec2(colW-6, 340), false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
                NS_WeaponList(cat, sel, colW-6);
                ImGui::EndChild();
                ImGui::NextColumn();
                ImGui::BeginChild("skin_grid_container", ImVec2(colW-6, 340), false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
                NS_WeaponSkinSection(sel, colW-6, needSync);
                ImGui::EndChild();
                ImGui::Columns(1);
                ImGui::Dummy(ImVec2(0, 12));
            }
        }
    }

    else if (activeTab == 4) {
        NS_Header("Hero Skins");
        // Online feature gating removed: every feature is always available.
        {
            NS_HeroSkinSection(cw - 4, needSync);
        }
    }

    else if (activeTab == 5) {
        NS_Header("Protection Config");
        ImGui::Columns(2, "prot_cols", false);
        ImGui::SetColumnWidth(0, colW);
        needSync |= NS_Toggle("prot_rec", "Hide on Record", &HIDEESP);
        needSync |= NS_Toggle("prot_ss", "Anti Screenshot", &sync_bool["bAntiScreenshot"]);
        ImGui::NextColumn();
        needSync |= NS_Toggle("prot_vpn", "Bypass VPN Check", &sync_bool["bBypassVPN"]);
        needSync |= NS_Toggle("safe_mode", "Safe Mode", &sync_bool["bSafeMode"]);
        needSync |= NS_Toggle("block_report", "Block Report", &sync_bool["bBlockReport"]);
        ImGui::Columns(1);
    }

    else if (activeTab == 6) {
        NS_Header("Settings");
        NS_Header("Theme Color");
        {
            float bs = 42.f;
            float cols_c[][3] = {
                {0.71f, 0.12f, 0.12f},
                {0.0f, 0.78f, 1.0f},
                {0.96f, 0.62f, 0.04f},
                {0.06f, 0.85f, 0.45f},
                {0.94f, 0.22f, 0.27f},
                {0.30f, 0.70f, 0.95f},
                {0.93f, 0.28f, 0.60f}
            };
            for (int i = 0; i < 7; i++) {
                if (i > 0) ImGui::SameLine(0, 8);
                ImVec2 p = ImGui::GetCursorScreenPos();
                ImU32 cc = IM_COL32((int)(cols_c[i][0]*255), (int)(cols_c[i][1]*255), (int)(cols_c[i][2]*255), 255);
                dl->AddCircleFilled(ImVec2(p.x+bs/2, p.y+bs/2), bs/2, cc);
                dl->AddCircle(ImVec2(p.x+bs/2, p.y+bs/2), bs/2, C_Border(), 0, 1.f);
                if (g_themeIdx == i) dl->AddCircle(ImVec2(p.x+bs/2, p.y+bs/2), bs/2+3, IM_COL32(255,255,255,150), 0, 1.5f);
                ImGui::PushID(i+600);
                ImGui::InvisibleButton("th", ImVec2(bs, bs));
                if (ImGui::IsItemClicked()) g_themeIdx = i;
                ImGui::PopID();
            }
        }
        ImGui::Dummy(ImVec2(0, 16));
        NS_Header("Config Save/Load");
        float bw = (cw - 16) / 3.f;
        if (NS_Button("save_cfg", "SAVE", ImVec2(bw, 32), true)) { SaveConfig(); AddNotification("Config saved!", 80, 220, 80, 2.f); }
        ImGui::SameLine(0, 8);
        if (NS_Button("load_cfg", "LOAD", ImVec2(bw, 32), false)) { LoadConfig(); AddNotification("Config loaded!", 80, 220, 80, 2.f); }
        ImGui::SameLine(0, 8);
        if (NS_Button("reset_cfg", "RESET", ImVec2(bw, 32), false)) { SetDefaultConfig(); SaveConfig(); AddNotification("Config reset!", 255, 80, 80, 2.f); }
        ImGui::Dummy(ImVec2(0, 16));
        NS_Header("About");
        NS_InfoRow("Version", APP_VERSION);
        NS_InfoRow("Developer", APP_MAKER);
        NS_InfoRow("Telegram", APP_CHANNEL);
        NS_InfoRow("Game", "Blood Strike");
    }

    else if (activeTab == 7) {
        NS_Header("Memory Features");

        ImVec2 warnPos = ImGui::GetCursorScreenPos();
        dl->AddRectFilled(warnPos, ImVec2(warnPos.x + cw, warnPos.y + 36), IM_COL32(255, 80, 80, 40), 4.f);
        dl->AddText(ImGui::GetFont(), 13.f, ImVec2(warnPos.x + 10, warnPos.y + 8), C_Danger(), "⚠ HIGH RISK");
        dl->AddText(ImGui::GetFont(), 11.f, ImVec2(warnPos.x + 10, warnPos.y + 24), C_TextDim(), "Direct memory modification. Use at your own risk!");
        ImGui::Dummy(ImVec2(0, 48));

        NS_Header("Player Mods");
        ImGui::Columns(2, "player_cols", false);
        ImGui::SetColumnWidth(0, colW);
        needSync |= NS_Toggle("player_god", "God Mode", &sync_bool["bGodMode"]);
        needSync |= NS_Toggle("player_antiknock", "Anti Knock", &sync_bool["bAntiKnock"]);
        needSync |= NS_Toggle("player_execrange", "Execute Range Hack", &sync_bool["bExecuteRangeHack"]);
        if (sync_bool["bExecuteRangeHack"]) {
            needSync |= NS_Slider("player_execrange_dist", "Execute Range", &sync_float["fExecuteRange"], 1.5f, 30.0f, "m");
        }
        needSync |= NS_Toggle("player_fly", "Fly / Terbang", &sync_bool["bFly"]);
        needSync |= NS_Toggle("player_teleport", "Teleport", &sync_bool["bTeleport"]);
        needSync |= NS_Toggle("player_nocooldown", "No Cooldown Skill", &sync_bool["bNoCooldown"]);
        needSync |= NS_Toggle("player_superjump", "Super Jump", &sync_bool["bSuperJump"]);
        needSync |= NS_Toggle("player_speed", "Speed Hack", &sync_bool["bSpeedHack"]);
        ImGui::NextColumn();
        needSync |= NS_Toggle("player_nogravity", "No Gravity", &sync_bool["bNoGravity"]);
        needSync |= NS_Toggle("player_nofalldamage", "No Fall Damage", &sync_bool["bNoFallDamage"]);
        needSync |= NS_Toggle("player_infinitestamina", "Infinite Stamina", &sync_bool["bInfiniteStamina"]);
        needSync |= NS_Toggle("player_waterwalk", "Water Walk", &sync_bool["bWaterWalk"]);
        needSync |= NS_Toggle("player_anstuck", "Air Stuck", &sync_bool["bAirStuck"]);
        ImGui::Columns(1);

        NS_Header("Player Sliders");
        ImGui::Columns(2, "player_sliders", false);
        ImGui::SetColumnWidth(0, colW);
        needSync |= NS_Slider("player_flyheight", "Fly Height", &sync_float["fFlyHeight"], 5.0f, 200.0f, "m");
        needSync |= NS_Slider("player_jumpmult", "Jump Multiplier", &sync_float["fJumpMult"], 1.0f, 5.0f, "x");
        needSync |= NS_Slider("player_speedmult", "Speed Multiplier", &sync_float["fSpeedMult"], 1.0f, 5.0f, "x");
        ImGui::NextColumn();
        needSync |= NS_Slider("player_teleportx", "Teleport X", &sync_float["fTeleportX"], -500.0f, 500.0f, "");
        needSync |= NS_Slider("player_teleporty", "Teleport Y", &sync_float["fTeleportY"], -500.0f, 500.0f, "");
        needSync |= NS_Slider("player_teleportz", "Teleport Z", &sync_float["fTeleportZ"], -500.0f, 500.0f, "");
        needSync |= NS_Slider("player_anstuckheight", "Air Stuck Height", &sync_float["fAirStuckHeight"], 5.0f, 200.0f, "m");
        ImGui::Columns(1);
        ImGui::Dummy(ImVec2(0, 12));

        NS_Header("Weapon Mods");
        ImGui::Columns(2, "weapon_cols", false);
        ImGui::SetColumnWidth(0, colW);
        needSync |= NS_Toggle("wep_no_recoil", "No Recoil", &sync_bool["bNoRecoil"]);
        needSync |= NS_Toggle("wep_no_spread", "No Spread", &sync_bool["bNoSpread"]);
        needSync |= NS_Toggle("wep_no_sway", "No Sway", &sync_bool["bNoSway"]);
        needSync |= NS_Toggle("wep_rapid_fire", "Rapid Fire", &sync_bool["bRapidFire"]);
        needSync |= NS_Toggle("wep_fast_reload", "Fast Reload", &sync_bool["bFastReload"]);
        needSync |= NS_Toggle("wep_instant_switch", "Instant Switch", &sync_bool["bInstantSwitch"]);
        ImGui::NextColumn();
        needSync |= NS_Toggle("wep_instant_scope", "Instant Scope (ADS)", &sync_bool["bInstantScope"]);
        needSync |= NS_Toggle("wep_infinite_ammo", "Infinite Ammo", &sync_bool["bInfiniteAmmo"]);
        ImGui::Columns(1);

        NS_Header("Weapon Sliders");
        ImGui::Columns(2, "weapon_sliders", false);
        ImGui::SetColumnWidth(0, colW);
        needSync |= NS_Slider("wep_fire_rate", "Fire Rate", &sync_float["fFireRateMult"], 0.5f, 5.0f, "x");
        needSync |= NS_Slider("wep_reload_speed", "Reload Speed", &sync_float["fReloadSpeedMult"], 0.5f, 5.0f, "x");
        needSync |= NS_Slider("wep_switch_speed", "Switch Speed", &sync_float["fSwitchSpeedMult"], 0.5f, 5.0f, "x");
        ImGui::NextColumn();
        needSync |= NS_Slider("wep_ads_speed", "ADS Speed", &sync_float["fADSSpeedMult"], 0.5f, 5.0f, "x");
        ImGui::Columns(1);
    }

    ImGui::EndChild();
    ImGui::End();
    ImGui::PopStyleColor(14);
    ImGui::PopStyleVar(2);
    if (needSync) sync_with_py();
}