#pragma once

#include "../AndInlineHook/AndInlineHook.h"

#include "hpp/imgui.h"
#include "hpp/imgui/touch.h"
#include "hpp/dlfcn.hpp"
#include "obfuscate.h"

#include <android/input.h>

#include "hpp/imgui/persistence.h"
#include "hpp/imgui/ctx_guard.h"

bool g_DraggingFromOutsideImGui = false;
int g_ActivePointerIndex = -1;

float (*__AMotionEvent_getY)(AInputEvent* motion_event, size_t pointer_index);
DEFINES(float, _AMotionEvent_getX, AInputEvent* motion_event, size_t pointer_index) {
    if (!_features["bBetterTouch"] || !__AMotionEvent_getY)
        return __AMotionEvent_getX(motion_event, pointer_index);

    float x = __AMotionEvent_getX(motion_event, pointer_index);
    float y = __AMotionEvent_getY(motion_event, pointer_index);
    
    int32_t action = AMotionEvent_getAction(motion_event);
    int32_t actionMasked = action & AMOTION_EVENT_ACTION_MASK;
    
    if (actionMasked == AMOTION_EVENT_ACTION_DOWN) {
        // [FIX] Cache hasil IsImGuiTouchPoint — jangan panggil 2x untuk koordinat sama
        bool onImGui = IsImGuiTouchPoint(x, y);
        if (!onImGui) {
            g_DraggingFromOutsideImGui = true;
            g_ActivePointerIndex = pointer_index;
        } else {
            g_DraggingFromOutsideImGui = false;
        }
    } else if (actionMasked == AMOTION_EVENT_ACTION_UP || actionMasked == AMOTION_EVENT_ACTION_CANCEL) {
        if (pointer_index == g_ActivePointerIndex) {
            g_DraggingFromOutsideImGui = false;
            g_ActivePointerIndex = -1;
        }
    }
    
    if (g_DraggingFromOutsideImGui && pointer_index == g_ActivePointerIndex) return x;
    
    // [FIX] Cache hasil IsImGuiTouchPoint — sebelumnya dipanggil 2x (di ACTION_DOWN + di sini)
    // Sekarang 1x aja, hasil disimpan ke var
    bool onImGui = IsImGuiTouchPoint(x, y);
    return onImGui ? -99999.0f : x;
}

float _AMotionEvent_getY(AInputEvent* motion_event, size_t pointer_index) {
    if (!_features["bBetterTouch"] || !__AMotionEvent_getX)
        return __AMotionEvent_getY(motion_event, pointer_index);

    float y = __AMotionEvent_getY(motion_event, pointer_index);
    float x = __AMotionEvent_getX(motion_event, pointer_index);
    
    if (g_DraggingFromOutsideImGui && pointer_index == g_ActivePointerIndex) return y;
    
    // [FIX] Cache IsImGuiTouchPoint result
    bool onImGui = IsImGuiTouchPoint(x, y);
    return onImGui ? -99999.0f : y;
}

DEFINES(void, InitializeMotionEvent, AInputEvent* event, const void* msg) {
    _InitializeMotionEvent(event, msg);
    if (!event) return;

    // Runs on the game's input thread and fires on EVERY motion event, including
    // before the first eglSwapBuffers created a context and after CleanupGL()
    // destroyed one. The ImGui Android backend calls ImGui::GetIO(), which
    // dereferences the (null) global context in release builds -- that is the
    // tap-to-crash. Forward only when a live context exists.
    if (!nx::imguiInputSafe()) return;

    ImGui_ImplAndroid_HandleInputEvent(event, {0.0f, 0.0f});
}

void __INPUT__() {
    HOOKIT("libinput.so", "_ZN7android13InputConsumer21initializeMotionEventEPNS_11MotionEventEPKNS_12InputMessageE", InitializeMotionEvent);
}
