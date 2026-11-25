#pragma once

#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include <android/native_activity.h>
#include <android/native_window.h>
#include <atomic>
#include <chrono>
#include <cstdint>
#include <functional>

#include "../imgui/imgui.h"
#include "../imgui/backends/imgui_impl_android.h"
#include "../imgui/backends/imgui_impl_opengl3.h"

namespace android {

// Owns a transparent SurfaceControl layer and its private EGL/ImGui context.
// The game EGL hook only drives this renderer; ImGui is never drawn into the
// game's framebuffer, so capture protection applies to the overlay alone.
class AImGui final {
public:
    struct Options {
        ANativeActivity* activity = nullptr;
        bool skipScreenshot = false;
        std::function<void()> onSetup = nullptr;
    };

    AImGui();
    explicit AImGui(const Options& options);
    ~AImGui();

    AImGui(const AImGui&) = delete;
    AImGui& operator=(const AImGui&) = delete;
    AImGui(AImGui&&) = delete;
    AImGui& operator=(AImGui&&) = delete;

    bool BeginFrame();
    bool EndFrame();
    void SetDisplaySize(int32_t width, int32_t height);

    // Toggleable at runtime. SurfaceControl updates the overlay layer only;
    // the Activity/game window is deliberately left captureable.
    bool SetHideFromCapture(bool hide);
    bool IsHiddenFromCapture() const;

    explicit operator bool() const { return m_state; }

private:
    bool InitEnvironment();
    void UnInitEnvironment();

    Options m_options;
    bool m_state = false;
    bool m_frameActive = false;
    bool m_androidBackendInitialized = false;
    bool m_openGLBackendInitialized = false;
    int32_t m_screenWidth = 0;
    int32_t m_screenHeight = 0;

    ANativeWindow* m_nativeWindow = nullptr;
    EGLDisplay m_display = EGL_NO_DISPLAY;
    EGLSurface m_surface = EGL_NO_SURFACE;
    EGLContext m_context = EGL_NO_CONTEXT;
    ImGuiContext* m_imguiContext = nullptr;

    std::atomic<bool> m_hideFromCapture{false};
    std::chrono::steady_clock::time_point m_lastSurfaceRefresh{};
};

} // namespace android