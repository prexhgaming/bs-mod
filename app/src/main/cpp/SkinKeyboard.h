// ============================================================
// SkinKeyboard.h — Virtual keyboard ImGui (overlay window)
// + Auto save/load dari /sdcard/SkinNames.txt
//
// Cara pakai di draw.h:
//   1. #include "SkinKeyboard.h"
//
//   2. Di GetSkinName lambda — ganti jadi:
//        auto GetSkinName = [&](int wid, int s) -> const char* {
//            // cari wname dari wid
//            const char* wn = nullptr;
//            for (int i=0;i<N_WEP;i++) if(WEAPONS[i].id==wid){wn=WEAPONS[i].name;break;}
//            if (wn) {
//                const char* cn = SkinKeyboard::GetCustomName(wn, s);
//                if (cn) return cn;
//            }
//            for (int i=0;i<N_SKIN_NAMES;i++)
//                if(SKIN_NAMES[i].id==wid) return SKIN_NAMES[i].names[s-1];
//            return "Skin ?";
//        };
//
//   3. Di dalam weapon body setelah skin buttons:
//        SkinKeyboard::DrawRenameBtn(wid, WEAPONS[w].name, cur_skin);
//
//   4. SETELAH } End(); menu utama:
//        SkinKeyboard::DrawKeyboard();
// ============================================================
#pragma once
#include "imgui/imgui.h"
#include <string>
#include <vector>
#include <fstream>
#include <functional>
#include <unordered_map>

namespace SkinKeyboard {

static constexpr const char* SAVE_PATH = "/sdcard/SkinNames.txt";

// ── State ─────────────────────────────────────────────────────
static std::string  s_input    = "";
static bool         s_caps     = false;
static bool         s_visible  = false;
static int          s_wid      = -1;
static int          s_slot     = -1;
static char         s_wname[32]  = "";
static char         s_notify[64] = "";
static float        s_notify_t   = 0.f;
static bool         s_loaded     = false;

// key = "WNAME_SLOT", contoh: "AK-47_5"
static std::unordered_map<std::string, std::string> s_names;

// ── Key helper ────────────────────────────────────────────────
static inline std::string MakeKey(const char* wname, int slot) {
    return std::string(wname) + "_" + std::to_string(slot);
}

// ── Load dari file ────────────────────────────────────────────
// Dipanggil otomatis sekali, parse semua baris //Ganti
static void LoadFromFile() {
    s_loaded = true;
    s_names.clear();
    std::ifstream f(SAVE_PATH);
    if (!f.is_open()) return;
    std::string line;
    while (std::getline(f, line)) {
        if (line.empty()) continue;
        // skip baris //default
        if (line.find("//default") != std::string::npos) continue;
        // format: "WNAME SLOT. NAME //Ganti"
        // cari ". "
        auto dot = line.find(". ");
        if (dot == std::string::npos) continue;
        // mundur dari dot cari spasi sebelum angka slot
        auto sp = line.rfind(' ', dot - 1);
        if (sp == std::string::npos) continue;
        std::string wname   = line.substr(0, sp);
        std::string slotStr = line.substr(sp + 1, dot - sp - 1);
        // validasi angka
        bool isNum = !slotStr.empty();
        for (char c : slotStr)
            if (!isdigit((unsigned char)c)) { isNum = false; break; }
        if (!isNum) continue;
        int slot = std::stoi(slotStr);
        // ambil nama antara ". " dan " //"
        size_t nameStart = dot + 2;
        size_t tagPos    = line.find(" //", nameStart);
        std::string name = (tagPos != std::string::npos)
            ? line.substr(nameStart, tagPos - nameStart)
            : line.substr(nameStart);
        if (!name.empty())
            s_names[wname + "_" + std::to_string(slot)] = name;
    }
}

// ── GetCustomName — dipanggil dari GetSkinName lambda ─────────
static inline const char* GetCustomName(const char* wname, int slot) {
    if (!s_loaded) LoadFromFile();
    auto it = s_names.find(MakeKey(wname, slot));
    if (it != s_names.end() && !it->second.empty())
        return it->second.c_str();
    return nullptr;
}

// ── Save entry ke file ────────────────────────────────────────
static void SaveEntry() {
    std::string key  = MakeKey(s_wname, s_slot);
    std::string line;
    if (s_input.empty()) {
        line         = std::string(s_wname) + " Skin" + std::to_string(s_slot) + " //default";
        s_names.erase(key);
    } else {
        line         = std::string(s_wname) + " " + std::to_string(s_slot)
                     + ". " + s_input + " //Ganti";
        s_names[key] = s_input;
    }

    // baca file lama, replace baris weapon+slot yang sama
    std::vector<std::string> lines;
    {
        std::ifstream f(SAVE_PATH);
        std::string l;
        while (std::getline(f, l))
            if (!l.empty()) lines.push_back(l);
    }
    std::string px1 = std::string(s_wname) + " Skin" + std::to_string(s_slot);
    std::string px2 = std::string(s_wname) + " " + std::to_string(s_slot) + ".";
    bool found = false;
    for (auto& l : lines)
        if (l.find(px1) == 0 || l.find(px2) == 0) { l = line; found = true; break; }
    if (!found) lines.push_back(line);

    std::ofstream f(SAVE_PATH, std::ios::trunc);
    for (auto& l : lines) f << l << "\n";

    snprintf(s_notify, sizeof(s_notify), "Saved!");
    s_notify_t = 2.f;
    s_input.clear();
    s_visible = false;
}

// ── Open keyboard ─────────────────────────────────────────────
static void Open(int wid, int slot, const char* wname) {
    if (!s_loaded) LoadFromFile();
    s_wid  = wid;
    s_slot = slot;
    snprintf(s_wname, sizeof(s_wname), "%s", wname);
    auto it = s_names.find(MakeKey(wname, slot));
    s_input = (it != s_names.end()) ? it->second : "";
    s_visible = true;
}

// ── Tombol Rename di weapon body ──────────────────────────────
static void DrawRenameBtn(int wid, const char* wname, int active_slot) {
    using namespace ImGui;
    if (active_slot <= 0) return;
    Spacing();
    PushStyleColor(ImGuiCol_Button,        ImVec4(0.25f,0.06f,0.45f,1.f));
    PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.40f,0.10f,0.65f,1.f));
    PushStyleColor(ImGuiCol_ButtonActive,  ImVec4(0.55f,0.13f,0.80f,1.f));
    char lbl[48];
    snprintf(lbl, sizeof(lbl), " Rename Skin %d##rn%d_%d", active_slot, wid, active_slot);
    if (Button(lbl, ImVec2(-1.f, 28.f)))
        Open(wid, active_slot, wname);
    PopStyleColor(3);
}

// ── Key button ────────────────────────────────────────────────
static void KB(const char* lbl, const char* lo, const char* hi,
               float w = 54.f, float h = 50.f)
{
    using namespace ImGui;
    PushStyleColor(ImGuiCol_Button,        ImVec4(0.15f,0.05f,0.28f,1.f));
    PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.28f,0.08f,0.50f,1.f));
    PushStyleColor(ImGuiCol_ButtonActive,  ImVec4(0.45f,0.12f,0.75f,1.f));
    if (Button(lbl, ImVec2(w, h)) && s_input.size() < 48)
        s_input += (s_caps ? hi : lo);
    PopStyleColor(3);
}

static void SpecialBtn(const char* lbl, float w, float h,
                       ImVec4 col, std::function<void()> fn)
{
    using namespace ImGui;
    PushStyleColor(ImGuiCol_Button,        col);
    PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(col.x+0.12f,col.y+0.04f,col.z+0.12f,1.f));
    PushStyleColor(ImGuiCol_ButtonActive,  ImVec4(col.x+0.20f,col.y+0.06f,col.z+0.20f,1.f));
    if (Button(lbl, ImVec2(w, h))) fn();
    PopStyleColor(3);
}

// ── Overlay keyboard window ───────────────────────────────────
static void DrawKeyboard() {
    if (!s_visible) return;
    using namespace ImGui;

    s_notify_t -= GetIO().DeltaTime;

    ImVec2 scr   = GetIO().DisplaySize;
    ImVec2 wsize(600.f, 400.f);
    SetNextWindowPos(
        ImVec2((scr.x - wsize.x) * 0.5f, (scr.y - wsize.y) * 0.5f),
        ImGuiCond_Always);
    SetNextWindowSize(wsize, ImGuiCond_Always);

    ImGuiWindowFlags flags =
          ImGuiWindowFlags_NoResize
        | ImGuiWindowFlags_NoMove
        | ImGuiWindowFlags_NoCollapse
        | ImGuiWindowFlags_NoScrollbar;

    PushStyleColor(ImGuiCol_WindowBg,      ImVec4(0.05f,0.02f,0.12f,0.98f));
    PushStyleColor(ImGuiCol_TitleBg,       ImVec4(0.22f,0.04f,0.45f,1.f));
    PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.32f,0.07f,0.60f,1.f));
    PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10.f, 10.f));
    PushStyleVar(ImGuiStyleVar_ItemSpacing,   ImVec2(4.f,  4.f));

    char title[64];
    snprintf(title, sizeof(title), " RENAME  %s  Slot %d##skbwnd", s_wname, s_slot);

    if (Begin(title, &s_visible, flags)) {

        // input display
        PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.04f,0.01f,0.10f,1.f));
        char disp[52]; snprintf(disp, sizeof(disp), "%s|", s_input.c_str());
        SetNextItemWidth(GetContentRegionAvail().x);
        InputText("##kbdisp", disp, sizeof(disp), ImGuiInputTextFlags_ReadOnly);
        PopStyleColor();

        if (s_notify_t > 0.f)
            TextColored(ImVec4(0.3f,1.f,0.4f,s_notify_t), "%s", s_notify);

        Spacing();

        // Row 1: 1-0
        const char* r1[] ={"1","2","3","4","5","6","7","8","9","0"};
        const char* r1u[]={"!","@","#","$","%","^","&","*","(",")"};
        for (int i=0;i<10;i++) { if (i>0) SameLine(0,4); KB(r1[i],r1[i],r1u[i]); }

        // Row 2: QWERTYUIOP
        Spacing();
        const char* r2l="qwertyuiop", *r2u="QWERTYUIOP";
        for (int i=0;i<10;i++) {
            if (i>0) SameLine(0,4);
            char lo[2]={r2l[i],0}, hi[2]={r2u[i],0}, lb[2]={(s_caps?r2u:r2l)[i],0};
            KB(lb,lo,hi);
        }

        // Row 3: ASDFGHJKL
        Spacing();
        SetCursorPosX(GetCursorPosX()+29.f);
        const char* r3l="asdfghjkl", *r3u="ASDFGHJKL";
        for (int i=0;i<9;i++) {
            if (i>0) SameLine(0,4);
            char lo[2]={r3l[i],0}, hi[2]={r3u[i],0}, lb[2]={(s_caps?r3u:r3l)[i],0};
            KB(lb,lo,hi);
        }

        // Row 4: ZXCVBNM + Backspace
        Spacing();
        SetCursorPosX(GetCursorPosX()+58.f);
        const char* r4l="zxcvbnm", *r4u="ZXCVBNM";
        for (int i=0;i<7;i++) {
            if (i>0) SameLine(0,4);
            char lo[2]={r4l[i],0}, hi[2]={r4u[i],0}, lb[2]={(s_caps?r4u:r4l)[i],0};
            KB(lb,lo,hi);
        }
        SameLine(0,4);
        SpecialBtn("<X##bk", 80.f, 50.f, ImVec4(0.35f,0.05f,0.12f,1.f),
            []{ if (!s_input.empty()) s_input.pop_back(); });

        // Row 5: CAP | SPACE | - | _ | CLR
        Spacing();
        PushStyleColor(ImGuiCol_Button,
            s_caps ? ImVec4(0.10f,0.42f,0.10f,1.f) : ImVec4(0.15f,0.05f,0.28f,1.f));
        PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.16f,0.58f,0.16f,1.f));
        PushStyleColor(ImGuiCol_ButtonActive,  ImVec4(0.20f,0.70f,0.20f,1.f));
        if (Button(s_caps?"CAP ON##cp":"CAP##cp", ImVec2(80.f,50.f))) s_caps = !s_caps;
        PopStyleColor(3);

        SameLine(0,4); KB("SPACE"," "," ",206.f,50.f);
        SameLine(0,4); KB("-","-","-",54.f,50.f);
        SameLine(0,4); KB("_","_","_",54.f,50.f);
        SameLine(0,4);
        SpecialBtn("CLR##cl", 80.f, 50.f, ImVec4(0.28f,0.04f,0.10f,1.f),
            []{ s_input.clear(); });

        // SAVE + CANCEL
        Spacing();
        float bw = (GetContentRegionAvail().x - 8.f) * 0.5f;
        PushStyleColor(ImGuiCol_Button,        ImVec4(0.07f,0.32f,0.52f,1.f));
        PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.10f,0.48f,0.75f,1.f));
        PushStyleColor(ImGuiCol_ButtonActive,  ImVec4(0.14f,0.60f,0.90f,1.f));
        if (Button(" SAVE##sv", ImVec2(bw,44.f))) SaveEntry();
        PopStyleColor(3);
        SameLine(0,8);
        PushStyleColor(ImGuiCol_Button,        ImVec4(0.28f,0.05f,0.05f,1.f));
        PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.45f,0.08f,0.08f,1.f));
        PushStyleColor(ImGuiCol_ButtonActive,  ImVec4(0.60f,0.10f,0.10f,1.f));
        if (Button("CANCEL##cn", ImVec2(bw,44.f))) { s_visible=false; s_input.clear(); }
        PopStyleColor(3);
    }
    End();
    PopStyleVar(2);
    PopStyleColor(3);
}

} // namespace SkinKeyboard
