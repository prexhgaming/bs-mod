#pragma once
#ifndef CONFIG_DIR
#define CONFIG_DIR "/storage/emulated/0/Nexora/"
#endif
#define INL_PATH "/storage/emulated/0/Nexora/imgui.ini"

#include <string>
#include <vector>
#include <unordered_map>
#include <chrono>
#include <ctime>
#include <algorithm>
#include <cmath>
#include <mutex>
#include <shared_mutex>
#include <dlfcn.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/system_properties.h>
#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include <fcntl.h>
#include <android/input.h>
#include <android/keycodes.h>
#include <thread>
#include <atomic>
#include <fstream>
#include <sstream>
#include <sys/mman.h>
#include <android/log.h>
#include <errno.h>
#include <android/native_window.h>

#include "include/hpp/imgui/persistence.h"
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_android.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "entity.h"
#include "Includes.h"
#include "AndInlineHook/AndInlineHook.h"   /* HOOKIT/DEFINES/HOOKI live here */
#include "fake_dlfcn.h"
#include "Dobby/dobby.h"
#include "include/hpp/dlfcn.hpp"
#include "include/hpp/imgui/touch.h"
#include "include/hpp/imgui/ctx_guard.h"
#include "include/sync_with_py.h"
#include "include/sync_entities.h"
#include "include/helpers.h"
#include "logo_image.h"
#include "include/obfuscate.h"

static void protectFunc(void* addr) {
    if (!addr) return;
    long pageSize = sysconf(_SC_PAGESIZE);
    uintptr_t pageStart = (uintptr_t)addr & ~(pageSize - 1);
    mprotect((void*)pageStart, pageSize, PROT_READ | PROT_WRITE | PROT_EXEC);
}

void ProcessTouchEvents() {
}

extern bool g_pending_cjk_reload;
extern bool g_cjkLoaded;
extern float global_anim_time;
extern GLuint g_logo_tex;
extern ANativeActivity* g_activity;
extern int Width, Height;
extern int screenWidth, screenHeight;
extern std::atomic<bool> aimgui_thread_ready;
extern bool windowOpen;
extern std::unordered_map<std::string, bool> sync_bool;
extern std::unordered_map<std::string, float> sync_float;
extern std::unordered_map<std::string, int> sync_int;
extern ImVec2 windowPos;
extern char FIFO_PATH[256];
extern std::atomic<bool> menu_visible;

static uint8_t g_logo_rgba[logo_width * logo_height * 4];
static bool setup = false;
static ImVec2 g_menu_size = {700.f, 500.f};
static ANativeActivity* g_nativeAct = nullptr;
static std::mutex g_touch_mutex;
static std::shared_mutex g_entity_mutex;
static std::recursive_mutex g_ImGuiMutex;
static bool isDragging = false;
static ImVec2 dragOffset = {0, 0};
static bool touchDown = false;
static ImVec2 lastTouchPos = {0, 0};
static ImVec2 g_miniFpsPos = {-1.f, -1.f};
static bool isDraggingFps = false;
static bool fpsPotentialDrag = false;
static ImVec2 fpsDragOffset = {0, 0};
static ImVec2 fpsDownPos = {0, 0};
static const float FPS_DRAG_THRESHOLD = 10.f;
static std::mutex g_font_mutex;

// ============================================================
// 🔥 ImGui renders on the GAME's thread and context
// ============================================================
// There is no overlay EGL context any more: Draw() is hooked onto
// eglSwapBuffers, which the game calls on its own render thread with its own
// context current, so ImGui paints into the game's back buffer. One thread,
// one context, one font atlas.

using namespace ImGui;
using namespace std;

#include "Colors.h"

static void iOS_GlassPanel(ImDrawList* dl, ImVec2 a, ImVec2 b, float rounding = 16.f, float alpha = 0.2f) {
    ImU32 blurColor = IM_COL32(255, 255, 255, (int)(std::max(alpha, 0.15f) * 255));
    dl->AddRectFilled(a, b, blurColor, rounding);
    dl->AddRect(a, b, IM_COL32(255, 255, 255, 20), rounding, 0, 0.5f);
    dl->AddRectFilledMultiColor(
        ImVec2(a.x + 4, a.y + 2), ImVec2(b.x - 4, a.y + rounding * 0.5f),
        IM_COL32(255, 255, 255, 30), IM_COL32(255, 255, 255, 30),
        IM_COL32(255, 255, 255, 0), IM_COL32(255, 255, 255, 0)
    );
}

static void noMore(ImDrawList* dl);
static void DrawMiniFps(ImDrawList* draw, float fps);
static void ClearNexoraImGuiWindowTracking();
static void ProcessEntities(int matchSeconds);

#include "include/Jni_Stuff.h"
#include "ESP/ESP.h"
#include "MENU.h"

#include "小鱼字体.h"

// ============================================================
bool LoadMandarinFontOnDemand() {
    ImGuiIO& io = ImGui::GetIO();
    if (g_cjkLoaded) return true;
    std::lock_guard<std::mutex> lock(g_font_mutex);
    if (g_cjkLoaded) return true;
    static bool s_loading = false;
    if (s_loading) return false;
    s_loading = true;
    bool success = false;
    
    if (!小鱼字体_data || 小鱼字体_size == 0) {
        s_loading = false;
        g_cjkLoaded = true;
        return false;
    }
    
    const unsigned char* fontBytes = reinterpret_cast<const unsigned char*>(小鱼字体_data);
    
    ImFontConfig cjkCfg;
    cjkCfg.SizePixels = 20.0f;
    cjkCfg.OversampleH = 2;
    cjkCfg.OversampleV = 2;
    cjkCfg.PixelSnapH = true;
    cjkCfg.MergeMode = true;
    cjkCfg.FontDataOwnedByAtlas = false;
    
    ImFontGlyphRangesBuilder builder;
    builder.AddRanges(io.Fonts->GetGlyphRangesChineseFull());
    ImVector<ImWchar> ranges;
    builder.BuildRanges(&ranges);
    
    if (ranges.Size > 0) {
        ImFont* cjkFont = io.Fonts->AddFontFromMemoryTTF(
            (void*)fontBytes,
            (int)小鱼字体_size,
             20.0f,
            &cjkCfg,
            ranges.Data
        );
        if (cjkFont) {
            success = true;
            g_cjkLoaded = true;
            io.Fonts->Build();
            ImGui_ImplOpenGL3_CreateFontsTexture();
        }
    }
    s_loading = false;
    if (!success) {
        if (io.Fonts->Fonts.empty()) {
            ImFontConfig defCfg;
            defCfg.SizePixels = 18.0f;
            defCfg.FontDataOwnedByAtlas = true;
            io.Fonts->AddFontDefault(&defCfg);
        }
        g_cjkLoaded = true;
    }
    return success;
}

// The config dir is ALWAYS the app's internal data dir. There is deliberately
// no external-storage fallback any more: it used to kick in whenever the
// activity was not known yet, and then C++ wrote to /storage/emulated/0/Nexora
// while the script looked at the internal path (which the sandbox may rewrite
// to its own root). sync_attrs stayed empty, so every feature whose Python-side
// default is False - bAIM, bESP_Skeleton - looked broken, while bESP (default
// True) kept drawing boxes. Callers now get an empty string until the activity
// exists and simply retry on the next frame.
static std::string g_configDir;   // resolved once, then cached

static inline std::string GetConfigDir() {
    if (!g_configDir.empty()) return g_configDir;
    if (!g_activity || !g_activity->internalDataPath) return std::string();
    g_configDir = std::string(g_activity->internalDataPath) + "/Nexora/";
    mkdir(g_configDir.c_str(), 0777);
    return g_configDir;
}

void SaveConfig() {
    try {
        nlohmann::json j;
        for (auto& [k, v] : sync_bool)  j["bool"][k]  = v;
        for (auto& [k, v] : sync_float) j["float"][k] = v;
        for (auto& [k, v] : sync_int)   j["int"][k]   = v;
        std::string dir = GetConfigDir();
        if (dir.empty()) return;
        std::ofstream f(dir + "config.json");
        if (f.is_open()) { f << j.dump(2); f.close(); }
    } catch (...) {}
}

void SetDefaultConfig() {
    sync_bool["bESP"] = sync_bool["bESP_Box"] = sync_bool["bESP_Health"] =
    sync_bool["bESP_Name"] = sync_bool["bESP_Distance"] = true;
    sync_bool["bESP_Skeleton"] = sync_bool["bESP_Weapon"] = false;
    sync_bool["bESP_EnemyAimWarning"] = sync_bool["bESP_InfoPanel"] = true;
    sync_bool["bESP_Radar"] = false; sync_bool["bESP_Armor"] = true;
    sync_bool["bAIM"] = sync_bool["bAIM_AutoTune"] = true;
    sync_bool["bAIM_Lane"] = true; sync_bool["bAIM_CheckVisibility"] = false;
    sync_bool["bAIM_IgnoreKnocked"] = true; sync_bool["bAIM_Prediction"] = true;
    sync_bool["bAIM_IgnoreBots"] = false;
    sync_float["fAIM_SnapStrength"] = 3.0f; sync_float["fAIM_BulletSpeed"] = 650.f;
    sync_float["fAIM_Fov"] = 120.f; sync_float["fAIM_MaxDist"] = 400.f;
    sync_int["iAIM_Trigger"] = 2; sync_int["iAIM_TargetBone"] = 1;
    sync_float["fESP_MaxDistance"] = 500.f; sync_float["fESP_RadarSize"] = 160.f;
    sync_float["fESP_RadarRange"] = 400.f;
    sync_bool["bItemESP"] = false; sync_float["fItemESP_MaxDist"] = 100.f;
    sync_bool["bESP_ProximityAlert"] = false; sync_bool["bWorldESP"] = false;
    sync_bool["bWESP_Vehicle"] = false; sync_bool["bWESP_Airdrop"] = false;
    sync_bool["bWESP_Supply"] = false; sync_float["fWorldESP_MaxDist"] = 300.f;
    sync_bool["bESP_FacingWarning"] = false;
    sync_bool["bNoRecoil"] = false; sync_bool["bNoSpread"] = false;
    sync_bool["bNoSway"] = false; sync_bool["bRapidFire"] = false;
    sync_bool["bFastReload"] = false; sync_bool["bInstantSwitch"] = false;
    sync_bool["bInstantScope"] = false; sync_bool["bInfiniteAmmo"] = false;
    sync_bool["bSniperNoSway"] = false; sync_bool["bSniperInfiniteBreath"] = false;
    sync_bool["bSniperNoZoom"] = false; sync_bool["bSniperFastReload"] = false;
    sync_bool["bSniperNoBreathShake"] = false; sync_bool["bSniperInstantScope"] = false;
    sync_bool["bSniperNoRecoil"] = false; sync_bool["bSniperNoSpread"] = false;
    sync_bool["bSniperNoFlinch"] = false; sync_bool["bSniperNoBob"] = false;
    sync_bool["bSniperPerfectAccuracy"] = false; sync_bool["bSniperZoomHack"] = false;
    sync_bool["bFOVHack"] = false; sync_bool["bNoBreath"] = false;
    sync_bool["bNoAimShake"] = false; sync_bool["bNoRecoilCam"] = false;
    sync_bool["bNoScopeSway"] = false; sync_bool["bThirdPerson"] = false;
    sync_bool["bZoomHack"] = false; sync_bool["bNoCamShake"] = false;
    sync_float["fFireRateMult"] = 1.0f; sync_float["fReloadSpeedMult"] = 1.0f;
    sync_float["fSwitchSpeedMult"] = 1.0f; sync_float["fADSSpeedMult"] = 1.0f;
    sync_float["fSniperReloadMult"] = 1.0f; sync_float["fSniperZoomValue"] = 1.0f;
    sync_float["fFOVValue"] = 90.0f; sync_float["fZoomValue"] = 1.0f;
    sync_float["fCamSpeedValue"] = 1.0f;
}

void LoadConfig() {
    try {
        std::string dir = GetConfigDir();
        if (dir.empty()) { SetDefaultConfig(); return; }
        std::ifstream f(dir + "config.json");
        if (!f.is_open()) { SetDefaultConfig(); return; }
        nlohmann::json j; f >> j; f.close();
        SetDefaultConfig();
        if (j.contains("bool")) for (auto& [k,v] : j["bool"].items()) if (v.is_boolean()) sync_bool[k] = v.get<bool>();
        if (j.contains("float")) for (auto& [k,v] : j["float"].items()) if (v.is_number()) sync_float[k] = v.get<float>();
        if (j.contains("int")) for (auto& [k,v] : j["int"].items()) if (v.is_number()) sync_int[k] = v.get<int>();
    } catch (...) { SetDefaultConfig(); }
}

static void CleanupGL() {
    if (g_logo_tex) { glDeleteTextures(1, &g_logo_tex); g_logo_tex = 0; }

    ImGui_ImplOpenGL3_Shutdown(); ImGui_ImplAndroid_Shutdown(); ImGui::DestroyContext();
}

static void PremiumStyleSetup() {
    ImGuiStyle& s = ImGui::GetStyle();
    s.WindowRounding = 20.f; s.ChildRounding = 12.f; s.FrameRounding = 10.f;
    s.GrabRounding = 8.f; s.PopupRounding = 16.f; s.ScrollbarRounding = 10.f;
    s.WindowPadding = ImVec2(16,16); s.FramePadding = ImVec2(10,8);
    s.ItemSpacing = ImVec2(12,10); s.ItemInnerSpacing = ImVec2(8,6);
    s.IndentSpacing = 22.f; s.ScrollbarSize = 18.f; s.GrabMinSize = 20.f;
    s.WindowBorderSize = s.ChildBorderSize = s.PopupBorderSize = s.FrameBorderSize = 0.f;
    s.Colors[ImGuiCol_Text] = ImVec4(1,1,1,1);
    s.Colors[ImGuiCol_WindowBg] = ImVec4(0.05f,0.04f,0.09f,0.98f);
}

static void NxPtclDraw(ImDrawList* dl, float w, float h, float dt) {
    if (!dl || w <= 0 || h <= 0) return;
    static float t = 0.f; t += dt;
    for (int i = 0; i < 12; i++)
        dl->AddCircleFilled({w*(0.1f+sinf(t*1.3f+i)*0.08f), h*(0.85f+cosf(t*1.7f+i*2.f)*0.05f)},
            2.f, IM_COL32(140,89,242,(int)(80+sinf(t*3.f+i)*40)));
}

static ImVec2 GetMiniFpsRect(float* outW = nullptr, float* outH = nullptr) {
    float w = 128.f, h = 56.f;
    if (outW) *outW = w; if (outH) *outH = h;
    if (g_miniFpsPos.x < 0.f || g_miniFpsPos.y < 0.f) {
        g_miniFpsPos.x = (float)Width - w - 25.f;
        g_miniFpsPos.y = (float)Height - h - 25.f;
    }
    if (Width > 0) g_miniFpsPos.x = std::max(0.f, std::min(g_miniFpsPos.x, (float)Width - w));
    if (Height > 0) g_miniFpsPos.y = std::max(0.f, std::min(g_miniFpsPos.y, (float)Height - h));
    return g_miniFpsPos;
}

static void DrawMiniFps(ImDrawList* draw, float fps) {
    if (!draw || Width <= 0 || Height <= 0) return;
    float w, h; ImVec2 pos = GetMiniFpsRect(&w, &h);
    float x = pos.x, y = pos.y;
    ImU32 bgCol = IM_COL32(22, 26, 36, 225);
    ImU32 borderCol = isDraggingFps ? IM_COL32(140, 89, 242, 200) : IM_COL32(70, 80, 100, 110);
    ImU32 textCol = fps >= 50.f ? IM_COL32(60, 230, 120, 255) : fps >= 30.f ? IM_COL32(255, 200, 60, 255) : IM_COL32(255, 80, 80, 255);
    float pillR = h * 0.5f;
    draw->AddRectFilled({x + 2, y + 3}, {x + w + 2, y + h + 3}, IM_COL32(0, 0, 0, 60), pillR);
    draw->AddRectFilled({x, y}, {x + w, y + h}, bgCol, pillR);
    draw->AddRect({x, y}, {x + w, y + h}, borderCol, pillR, 0, isDraggingFps ? 2.f : 1.2f);
    float cx = x + 30.f, cy = y + h * 0.5f, radius = 21.f;
    draw->AddCircleFilled({cx, cy}, radius, IM_COL32(40, 45, 60, 255));
    draw->AddCircle({cx, cy}, radius, IM_COL32(255, 255, 255, 60), 0, 2.f);
    if (g_logo_tex) {
        draw->AddImageRounded((ImTextureID)(intptr_t)g_logo_tex,
            {cx - radius + 4.f, cy - radius + 4.f}, {cx + radius - 4.f, cy + radius - 4.f},
            {0, 0}, {1, 1}, IM_COL32(255, 255, 255, 255), radius - 4.f);
    } else {
        draw->AddText(ImGui::GetFont(), 24.f, {cx - 6.f, cy - 12.f}, IM_COL32(255, 255, 255, 200), "N");
    }
    char buf[16]; snprintf(buf, sizeof(buf), "%.0f FPS", fps);
    ImVec2 textSize = ImGui::CalcTextSize(buf);
    draw->AddText(ImGui::GetFont(), 22.f, ImVec2(x + 60.f, y + (h - textSize.y) * 0.5f), textCol, buf);
}

int32_t (*orig_ANativeWindow_getWidth)(ANativeWindow *window);
int32_t _ANativeWindow_getWidth(ANativeWindow *window) {
    screenWidth = orig_ANativeWindow_getWidth(window);
    return orig_ANativeWindow_getWidth(window);
}
int32_t (*orig_ANativeWindow_getHeight)(ANativeWindow *window);
int32_t _ANativeWindow_getHeight(ANativeWindow *window) {
    screenHeight = orig_ANativeWindow_getHeight(window);
    return orig_ANativeWindow_getHeight(window);
}

ImVec2 SmoothMove(ImVec2 start, ImVec2 end, float speed) {
    return ImVec2(start.x + (end.x - start.x) * speed, start.y + (end.y - start.y) * speed);
}

int32_t (*orig_AInputQueue_getEvent)(AInputQueue *queue, AInputEvent **outEvent);
int32_t hooked_AInputQueue_getEvent(AInputQueue *queue, AInputEvent **outEvent) {
    int32_t res = orig_AInputQueue_getEvent(queue, outEvent);
    // Replaces the bare `setup` flag: this runs on the game's input thread, where
    // the ImGui context can be null (before the first frame rendered, or after
    // CleanupGL destroyed it) and ImGui::GetIO() would dereference null.
    if (res >= 0 && nx::imguiInputSafe() && outEvent && *outEvent) {
        int eventType = AInputEvent_getType(*outEvent);
        if (eventType == AINPUT_EVENT_TYPE_MOTION) {
            int action = AMotionEvent_getAction(*outEvent) & AMOTION_EVENT_ACTION_MASK;
            float x = AMotionEvent_getX(*outEvent, 0);
            float y = AMotionEvent_getY(*outEvent, 0);
            float normX = (screenWidth  > 0) ? (x / (float)screenWidth)  * Width  : x;
            float normY = (screenHeight > 0) ? (y / (float)screenHeight) * Height : y;
            
            ImGuiIO & io = ImGui::GetIO();
            io.MousePos = ImVec2(normX, normY);
            
            switch (action) {
                case AMOTION_EVENT_ACTION_DOWN:
                    io.MouseDown[0] = true;
                    touchDown = true;
                    lastTouchPos = ImVec2(normX, normY);
                    if (menu_visible.load() &&
                        normX >= windowPos.x && normX <= windowPos.x + 820 &&
                        normY >= windowPos.y && normY <= windowPos.y + 68) {
                        isDragging = true;
                        dragOffset = ImVec2(normX - windowPos.x, normY - windowPos.y);
                    }
                    if (!menu_visible.load() && Width > 0 && Height > 0) {
                        float fw, fh;
                        ImVec2 fpsPos = GetMiniFpsRect(&fw, &fh);
                        if (normX >= fpsPos.x && normX <= fpsPos.x + fw &&
                            normY >= fpsPos.y && normY <= fpsPos.y + fh) {
                            fpsPotentialDrag = true;
                            fpsDownPos = ImVec2(normX, normY);
                            fpsDragOffset = ImVec2(normX - fpsPos.x, normY - fpsPos.y);
                            return 1;
                        }
                    }
                    break;
                case AMOTION_EVENT_ACTION_MOVE:
                    if (isDragging && menu_visible.load()) {
                        ImVec2 targetPos = ImVec2(normX - dragOffset.x, normY - dragOffset.y);
                        targetPos.x = std::max(0.f, std::min(targetPos.x, (float)Width - 820.f));
                        targetPos.y = std::max(0.f, std::min(targetPos.y, (float)Height - 620.f));
                        windowPos = SmoothMove(windowPos, targetPos, 0.25f);
                    }
                    if (fpsPotentialDrag && !menu_visible.load()) {
                        float dx = normX - fpsDownPos.x, dy = normY - fpsDownPos.y;
                        if (isDraggingFps || (dx*dx + dy*dy) > (FPS_DRAG_THRESHOLD * FPS_DRAG_THRESHOLD)) {
                            isDraggingFps = true;
                            float fw, fh;
                            GetMiniFpsRect(&fw, &fh);
                            ImVec2 targetPos = ImVec2(normX - fpsDragOffset.x, normY - fpsDragOffset.y);
                            targetPos.x = std::max(0.f, std::min(targetPos.x, (float)Width  - fw));
                            targetPos.y = std::max(0.f, std::min(targetPos.y, (float)Height - fh));
                            g_miniFpsPos = targetPos;
                        }
                    }
                    break;
                case AMOTION_EVENT_ACTION_UP:
                case AMOTION_EVENT_ACTION_CANCEL:
                    io.MouseDown[0] = false;
                    touchDown = false;
                    isDragging = false;
                    if (fpsPotentialDrag && !isDraggingFps) {
                        bool newState = !menu_visible.load();
                        menu_visible = newState;
                        if (newState) {
                            AddNotification("Menu Shown", 140, 89, 242, 1.5f);
                        } else {
                            AddNotification("Menu Hidden", 255, 180, 40, 1.5f);
                        }
                    }
                    fpsPotentialDrag = false;
                    isDraggingFps = false;
                    break;
            }
            return 1;
        }
    }
    return res;
}

DEFINES(EGLBoolean, Draw, EGLDisplay dpy, EGLSurface surface) {
    // A swap can arrive with no real surface (shutdown, surface teardown); pass
    // it straight through instead of querying or drawing on it.
    if (dpy == EGL_NO_DISPLAY || surface == EGL_NO_SURFACE) return _Draw(dpy, surface);

    eglQuerySurface(dpy, surface, EGL_WIDTH, &Width);
    eglQuerySurface(dpy, surface, EGL_HEIGHT, &Height);
    if (Width <= 0 || Height <= 0) return _Draw(dpy, surface);

    sync_float["fWidth"] = (float)Width;
    sync_float["fHeight"] = (float)Height;
    sync_with_py_immediate();  // snapshot the live values into data.json

    static bool imgui_context_ready = false;

    // CleanupGL() calls ImGui::DestroyContext(), which leaves no current
    // context, so this can be null even after the first frame. Rebuild it
    // instead of dereferencing a null GImGui further down.
    if (!ImGui::GetCurrentContext()) {
        nx::imguiBeginTeardown();
        setup = false;
        imgui_context_ready = false;
    }

    if (!imgui_context_ready) {
        imgui_context_ready = true;

        if (!ImGui::CreateContext()) {
            // Allocation failed: stay un-ready so input keeps being ignored and the
            // next frame retries, rather than crashing on a null context.
            imgui_context_ready = false;
            return _Draw(dpy, surface);
        }
        ImGui_ImplOpenGL3_Init("#version 300 es");
        // There is no overlay ANativeWindow any more: ImGui draws into the game's
        // own back buffer. Init with nullptr so the backend keeps the key map /
        // platform setup, and let NewFrame() fall back to the eglQuerySurface
        // size that Draw() passes in.
        ImGui_ImplAndroid_Init(nullptr);

        if (!g_logo_tex) {
            glGenTextures(1, &g_logo_tex);
            glBindTexture(GL_TEXTURE_2D, g_logo_tex);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, logo_width, logo_height, 0,
                         GL_RGBA, GL_UNSIGNED_BYTE, logo_data);
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_IsTouchScreen;

        ImFontConfig defCfg;
        defCfg.SizePixels = 18.0f;
        defCfg.FontDataOwnedByAtlas = true;
        io.Fonts->AddFontDefault(&defCfg);

        if (小鱼字体_data && 小鱼字体_size > 0) {
            const unsigned char* fontBytes = reinterpret_cast<const unsigned char*>(小鱼字体_data);
            ImFontConfig cjkCfg;
            cjkCfg.SizePixels = 20.0f;
            cjkCfg.OversampleH = 2;
            cjkCfg.OversampleV = 2;
            cjkCfg.PixelSnapH = true;
            cjkCfg.MergeMode = true;
            cjkCfg.FontDataOwnedByAtlas = false;
            ImFont* cjkFont = io.Fonts->AddFontFromMemoryTTF(
                (void*)fontBytes,
                (int)小鱼字体_size,
                 20.0f,
                &cjkCfg,
                io.Fonts->GetGlyphRangesChineseFull()
            );
            if (cjkFont) {
                g_cjkLoaded = true;
            }
        }

        io.Fonts->Build();
        ImGui_ImplOpenGL3_CreateFontsTexture();
        
        g_cjkLoaded = false;
        g_pending_cjk_reload = false;
        
        LoadConfig();
        
        sync_with_py_immediate();  // snapshot the live values into data.json
        aimgui_thread_ready = true;
        // Publish readiness only now, once the context and its font atlas exist,
        // so the input thread may safely touch ImGui again.
        setup = true;
        nx::imguiReady().store(true, std::memory_order_release);
        AddNotification("Nexora Loaded", 140, 89, 242, 3.0f);
    }

    if (g_pending_cjk_reload && !g_cjkLoaded) {
        g_pending_cjk_reload = false;
        LoadMandarinFontOnDemand();
    }

    ImGuiIO& io = ImGui::GetIO();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplAndroid_NewFrame(Width, Height);
    ImGui::NewFrame();
    ImDrawList* draw = ImGui::GetBackgroundDrawList();
    if (!draw) { ImGui::Render(); return _Draw(dpy, surface); }

    const std::vector<Entity>& ents = get_frame_entities();
    static auto ptlast = std::chrono::steady_clock::now();
    auto  ptnow = std::chrono::steady_clock::now();
    float dt    = std::chrono::duration_cast<std::chrono::milliseconds>(ptnow-ptlast).count()/1000.f;
    ptlast = ptnow;
    if (dt > 0.05f) dt = 0.05f;
    global_anim_time += dt;

    if (menu_visible.load()) NxPtclDraw(draw, (float)Width, (float)Height, dt);

    ESP::DrawAll(draw, ents);
    DrawNotifications();

    if (menu_visible.load()) {
        ImGui::SetNextWindowPos(windowPos, ImGuiCond_Always);
        DrawMenu(ImGui::GetIO());
    } else {
        DrawMiniFps(draw, io.Framerate);
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    return _Draw(dpy, surface);
}

// ============================================================
// 🔥 IMGUI() - INSTALL THE SWAP HOOK
// ============================================================
void IMGUI() {
    start_entity_reader();

    void *libAndroid = dlopen("libandroid.so", RTLD_NOW);
    if (libAndroid) {
        void *symEvent1 = dlsym(libAndroid, "ANativeWindow_getWidth");
        DobbyHook(symEvent1, (void *)_ANativeWindow_getWidth, (void **)&orig_ANativeWindow_getWidth);
        void *symEvent2 = dlsym(libAndroid, "ANativeWindow_getHeight");  
        DobbyHook(symEvent2, (void *)_ANativeWindow_getHeight, (void **)&orig_ANativeWindow_getHeight);  
        void *symEvent = dlsym(libAndroid, "AInputQueue_getEvent");  
        protectFunc(symEvent);  
        DobbyHook(symEvent, (void *)hooked_AInputQueue_getEvent, (void **)&orig_AInputQueue_getEvent);  
    }

    // ============================================================
    // 🔥 HOOK eglSwapBuffers -> Draw()
    // ============================================================
    // Resolved through Dobby's own symbol resolver rather than fake_dlfcn +
    // AndInlineHook, so there is exactly one hooking engine in the picture.
    //
    // The game calls eglSwapBuffers on ITS OWN render thread with ITS OWN EGL
    // context current, so Draw() paints ImGui straight into the game's back
    // buffer before the swap. That means one thread, one EGL context and one
    // font atlas for the whole process. The previous overlay owned a second
    // EGL context plus a full-screen SurfaceControl window surface (3 screen-
    // sized buffers) whose BeginFrame()/EndFrame() were never called, so all of
    // it was allocated and never drew a single pixel.
    void *eglSwapBuffersAddr = DobbySymbolResolver("libEGL.so", "eglSwapBuffers");
    if (!eglSwapBuffersAddr) {
        // Some vendor builds only export it from the global namespace.
        eglSwapBuffersAddr = dlsym(RTLD_DEFAULT, "eglSwapBuffers");
    }
    if (!eglSwapBuffersAddr) {
        LOGE("eglSwapBuffers not found - menu will not render");
        AddNotification("❌ Swap hook failed!", 255, 80, 80, 3.0f);
        return;
    }

    int hookResult = DobbyHook(eglSwapBuffersAddr, (void *)Draw, (void **)&_Draw);
    if (hookResult != 0 || !_Draw) {
        LOGE("eglSwapBuffers hook failed (%d) at %p", hookResult, eglSwapBuffersAddr);
        AddNotification("❌ Swap hook failed!", 255, 80, 80, 3.0f);
        return;
    }

    AddNotification("✅ Nexora Ready", 80, 220, 80, 2.0f);
    LOGI("eglSwapBuffers hooked at %p", eglSwapBuffersAddr);
}

__attribute__((destructor))
static void OnExit() {
    // Flip this before tearing GL down, so input arriving during shutdown is
    // dropped instead of touching a half-destroyed context.
    nx::imguiBeginTeardown();
    try { SaveConfig(); CleanupGL(); } catch (...) {}
}
