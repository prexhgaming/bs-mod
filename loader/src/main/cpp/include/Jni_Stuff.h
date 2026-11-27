#pragma once

#include "obfuscate.h"
#include "../nlohmann/json.hpp"
#include "../Oxorany/oxorany.h"
#include <sstream>
#include <iomanip>
#include <cstring>
#include <cstdio>
#include <sys/stat.h>
#include <atomic>
#include <thread>
#include <fstream>
#include <algorithm>
#include <mutex>
#include <vector>
#include <string>
#include <android/log.h>

extern ANativeActivity* g_activity;
extern int Width, Height;
extern float global_anim_time;
extern bool g_pending_cjk_reload;
extern GLuint g_logo_tex;

extern void AddNotification(const char* msg, int r, int g, int b, float dur);
extern void SaveConfig();
extern void SetDefaultConfig();
extern void LoadConfig();
extern bool LoadMandarinFontOnDemand();

float g_ui_scale = 1.0f;
bool g_show_menu = true;
bool g_show_notifications = true;
bool g_show_floating = true;

bool g_feature_esp = true;
bool g_feature_item = true;
bool g_feature_aim = true;
bool g_feature_silent_aim = true;
bool g_feature_bullet_track = true;
bool g_feature_floating = true;
bool g_feature_memory = true;
bool g_feature_setting = true;

ImU32 g_menu_color = IM_COL32(0, 180, 255, 200);
ImU32 g_menu_bg = IM_COL32(15, 18, 22, 230);
ImU32 g_menu_accent = IM_COL32(0, 180, 255, 255);
ImU32 g_menu_red = IM_COL32(255, 69, 58, 255);
ImU32 g_menu_green = IM_COL32(52, 199, 89, 255);
ImU32 g_menu_yellow = IM_COL32(255, 149, 0, 255);

#define XSTR(s) OBFUSCATE(s)

static const char* XOR_MENU_TITLE       = OBFUSCATE("NEXORA MOD MENU");
static const char* XOR_MENU_USER        = OBFUSCATE("User: ");
static const char* XOR_MENU_EXPIRES     = OBFUSCATE("Expires: ");
static const char* XOR_MENU_DAYS_LEFT   = OBFUSCATE("days left");
static const char* XOR_MENU_MOD         = OBFUSCATE("Mod: ");
static const char* XOR_MENU_ESP         = OBFUSCATE("ESP");
static const char* XOR_MENU_ITEM        = OBFUSCATE("Item ESP");
static const char* XOR_MENU_AIM         = OBFUSCATE("AIM Assist");
static const char* XOR_MENU_SILENT      = OBFUSCATE("Silent AIM");
static const char* XOR_MENU_BULLET      = OBFUSCATE("Bullet Track");
static const char* XOR_MENU_FLOATING    = OBFUSCATE("Floating");
static const char* XOR_MENU_CLOSE       = OBFUSCATE("X");
static const char* XOR_MENU_COPYRIGHT   = OBFUSCATE("© 2026 NEXORA");
static const char* XOR_MENU_VERSION     = OBFUSCATE("v2.0");
static const char* XOR_MENU_ERROR       = OBFUSCATE("ERROR: ");
static const char* XOR_MENU_WARNING     = OBFUSCATE("⚠️ NOTIFICATION");
static const char* XOR_MENU_BLOODSTRIKE = OBFUSCATE("BLOODSTRIKE EXTERNAL");
static const char* XOR_MENU_NEXORA      = OBFUSCATE("NEXORA");

using json = nlohmann::json;

struct Notification {
    std::string message;
    ImU32 color;
    float time;
    float duration;
};

static std::vector<Notification> g_notifications;
static std::mutex g_notification_mutex;

void AddNotification(const char* msg, int r, int g, int b, float dur) {
    std::lock_guard<std::mutex> lock(g_notification_mutex);
    Notification notif;
    notif.message = msg;
    notif.color = IM_COL32(r, g, b, 255);
    notif.time = 0.0f;
    notif.duration = dur;
    g_notifications.push_back(notif);
    
    if (g_notifications.size() > 10) {
        g_notifications.erase(g_notifications.begin());
    }
}

void ClearNotifications() {
    std::lock_guard<std::mutex> lock(g_notification_mutex);
    g_notifications.clear();
}





void DrawNotifications() {
    using namespace ImGui;
    
    float scale = (g_ui_scale > 0.f) ? g_ui_scale : 1.f;
    float sw = GetIO().DisplaySize.x;
    float sh = GetIO().DisplaySize.y;
    
    float y_offset = 60 * scale;
    float x_offset = sw - 340 * scale;
    
    ImDrawList* dl = GetForegroundDrawList();
    
    for (auto it = g_notifications.begin(); it != g_notifications.end();) {
        it->time += GetIO().DeltaTime;
        
        float alpha = 1.0f;
        if (it->time > it->duration - 0.5f) {
            alpha = (it->duration - it->time) / 0.5f;
        }
        if (it->time > it->duration) {
            it = g_notifications.erase(it);
            continue;
        }
        
        ImU32 color = it->color;
        ImU32 bgColor = IM_COL32(15, 18, 22, (int)(200 * alpha));
        ImU32 borderColor = IM_COL32(
            (color >> 16) & 0xFF,
            (color >> 8) & 0xFF,
            color & 0xFF,
            (int)(150 * alpha)
        );
        
        ImVec2 textSize = CalcTextSize(it->message.c_str());
        float notif_w = textSize.x + 40 * scale;
        float notif_h = textSize.y + 20 * scale;
        float notif_x = sw - notif_w - 20 * scale;
        float notif_y = y_offset;
        
        dl->AddRectFilled(
            {notif_x, notif_y},
            {notif_x + notif_w, notif_y + notif_h},
            bgColor, 10 * scale
        );
        
        dl->AddRect(
            {notif_x, notif_y},
            {notif_x + notif_w, notif_y + notif_h},
            borderColor, 10 * scale, 0, 1.5f * scale
        );
        
        dl->AddText(
            ImGui::GetFont(), 14 * scale,
            {notif_x + 20 * scale, notif_y + 10 * scale},
            IM_COL32(255, 255, 255, (int)(255 * alpha)),
            it->message.c_str()
        );
        
        y_offset += notif_h + 8 * scale;
        ++it;
    }
}
