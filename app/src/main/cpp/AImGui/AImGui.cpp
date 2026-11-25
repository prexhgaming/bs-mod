#include "AImGui.hpp"

#include "../ANwCreator.hpp"

#include <EGL/eglext.h>
#include <android/log.h>

#ifndef EGL_OPENGL_ES3_BIT
#define EGL_OPENGL_ES3_BIT 0x00000040
#endif

namespace {

constexpr const char* kLogTag = "SecureAImGui";

#define AIMGUI_LOGE(...) __android_log_print(ANDROID_LOG_ERROR, kLogTag, __VA_ARGS__)
#define AIMGUI_LOGI(...) __android_log_print(ANDROID_LOG_INFO, kLogTag, __VA_ARGS__)

struct EGLState {
    EGLDisplay display = eglGetCurrentDisplay();
    EGLSurface drawSurface = eglGetCurrentSurface(EGL_DRAW);
    EGLSurface readSurface = eglGetCurrentSurface(EGL_READ);
    EGLContext context = eglGetCurrentContext();

    void Restore() const {
        if (display != EGL_NO_DISPLAY) {
            eglMakeCurrent(display, drawSurface, readSurface, context);
        }
    }
};

} // namespace

namespace android {

AImGui::AImGui() : AImGui(Options{}) {}

AImGui::AImGui(const Options& options) : m_options(options) {
    m_hideFromCapture.store(options.skipScreenshot, std::memory_order_relaxed);
    m_state = InitEnvironment();
}

AImGui::~AImGui() {
    UnInitEnvironment();
}

void AImGui::SetDisplaySize(int32_t width, int32_t height) {
    if (width <= 0 || height <= 0 ||
        (width == m_screenWidth && height == m_screenHeight)) {
        return;
    }

    m_screenWidth = width;
    m_screenHeight = height;
    if (m_nativeWindow) {
        // Resize only on a real orientation/surface-size change; doing this
        // every frame can force buffer reallocation and cause micro-stutter.
        ANativeWindow_setBuffersGeometry(
            m_nativeWindow,
            width,
            height,
            ANativeWindow_getFormat(m_nativeWindow));
    }
}

bool AImGui::SetHideFromCapture(bool hide) {
    if (!m_nativeWindow || !m_options.activity) {
        return false;
    }

    const bool applied = ANwCreator::SetCaptureProtection(
        m_options.activity,
        m_nativeWindow,
        hide);
    if (applied) {
        m_hideFromCapture.store(hide, std::memory_order_release);
    }
    return applied;
}

bool AImGui::IsHiddenFromCapture() const {
    return m_hideFromCapture.load(std::memory_order_acquire);
}

bool AImGui::InitEnvironment() {
    if (!m_options.activity || !m_options.activity->vm || !m_options.activity->clazz) {
        AIMGUI_LOGE("No active Android Activity");
        return false;
    }

    const EGLState previousEGL;

    const ANwCreator::DisplayInfo displayInfo =
        ANwCreator::GetDisplayInfo(m_options.activity);
    m_screenWidth = displayInfo.width;
    m_screenHeight = displayInfo.height;
    if (m_screenWidth <= 0 || m_screenHeight <= 0) {
        AIMGUI_LOGE("Invalid display size: %d x %d", m_screenWidth, m_screenHeight);
        return false;
    }

    ANwCreator::CreateOptions windowOptions;
    windowOptions.name = "KONAB_ImGui_Overlay";
    windowOptions.width = m_screenWidth;
    windowOptions.height = m_screenHeight;
    // Apply the saved state before the first visible overlay frame. The same
    // transaction flag remains toggleable without rebuilding the EGL context.
    windowOptions.skipScreenshot = m_options.skipScreenshot;
    m_nativeWindow = ANwCreator::Create(m_options.activity, windowOptions);
    if (!m_nativeWindow) {
        AIMGUI_LOGE("Unable to create overlay SurfaceControl");
        return false;
    }

    m_display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (m_display == EGL_NO_DISPLAY || !eglInitialize(m_display, nullptr, nullptr)) {
        AIMGUI_LOGE("eglInitialize failed: 0x%x", eglGetError());
        UnInitEnvironment();
        previousEGL.Restore();
        return false;
    }

    const EGLint configAttributes[] = {
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT,
        EGL_RED_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_BLUE_SIZE, 8,
        EGL_ALPHA_SIZE, 8,
        EGL_DEPTH_SIZE, 0,
        EGL_STENCIL_SIZE, 0,
        EGL_NONE
    };

    EGLConfig config = nullptr;
    EGLint configCount = 0;
    if (!eglChooseConfig(m_display, configAttributes, &config, 1, &configCount) ||
        configCount < 1) {
        AIMGUI_LOGE("eglChooseConfig failed: 0x%x", eglGetError());
        UnInitEnvironment();
        previousEGL.Restore();
        return false;
    }

    EGLint nativeFormat = 0;
    if (eglGetConfigAttrib(m_display, config, EGL_NATIVE_VISUAL_ID, &nativeFormat)) {
        ANativeWindow_setBuffersGeometry(m_nativeWindow, 0, 0, nativeFormat);
    }

    m_surface = eglCreateWindowSurface(m_display, config, m_nativeWindow, nullptr);
    if (m_surface == EGL_NO_SURFACE) {
        AIMGUI_LOGE("eglCreateWindowSurface failed: 0x%x", eglGetError());
        UnInitEnvironment();
        previousEGL.Restore();
        return false;
    }

    const EGLint contextAttributes[] = {
        EGL_CONTEXT_CLIENT_VERSION, 3,
        EGL_NONE
    };
    m_context = eglCreateContext(m_display, config, EGL_NO_CONTEXT, contextAttributes);
    if (m_context == EGL_NO_CONTEXT) {
        AIMGUI_LOGE("eglCreateContext failed: 0x%x", eglGetError());
        UnInitEnvironment();
        previousEGL.Restore();
        return false;
    }

    if (!eglMakeCurrent(m_display, m_surface, m_surface, m_context)) {
        AIMGUI_LOGE("eglMakeCurrent failed: 0x%x", eglGetError());
        UnInitEnvironment();
        previousEGL.Restore();
        return false;
    }

    // The game surface owns pacing/vsync. A second blocking swap here would
    // halve the effective frame rate on many devices.
    eglSwapInterval(m_display, 0);

    IMGUI_CHECKVERSION();
    m_imguiContext = ImGui::CreateContext();
    if (!m_imguiContext) {
        AIMGUI_LOGE("ImGui::CreateContext failed");
        UnInitEnvironment();
        previousEGL.Restore();
        return false;
    }
    ImGui::SetCurrentContext(m_imguiContext);

    if (!ImGui_ImplAndroid_Init(m_nativeWindow)) {
        AIMGUI_LOGE("ImGui Android backend initialization failed");
        UnInitEnvironment();
        previousEGL.Restore();
        return false;
    }
    m_androidBackendInitialized = true;

    if (!ImGui_ImplOpenGL3_Init("#version 300 es")) {
        AIMGUI_LOGE("ImGui OpenGL backend initialization failed");
        UnInitEnvironment();
        previousEGL.Restore();
        return false;
    }
    m_openGLBackendInitialized = true;

    if (m_options.onSetup) {
        m_options.onSetup();
    }

    glViewport(0, 0, m_screenWidth, m_screenHeight);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    if (!SetHideFromCapture(m_options.skipScreenshot)) {
        AIMGUI_LOGE("Capture-protection transaction was unavailable");
    }

    m_lastSurfaceRefresh = std::chrono::steady_clock::now();
    previousEGL.Restore();
    AIMGUI_LOGI("Secure overlay ready at %d x %d", m_screenWidth, m_screenHeight);
    return true;
}

bool AImGui::BeginFrame() {
    if (!m_state || m_frameActive || m_display == EGL_NO_DISPLAY ||
        m_surface == EGL_NO_SURFACE || m_context == EGL_NO_CONTEXT ||
        !m_imguiContext) {
        return false;
    }

    if (!eglMakeCurrent(m_display, m_surface, m_surface, m_context)) {
        AIMGUI_LOGE("BeginFrame eglMakeCurrent failed: 0x%x", eglGetError());
        return false;
    }

    ImGui::SetCurrentContext(m_imguiContext);

    const auto now = std::chrono::steady_clock::now();
    if (now - m_lastSurfaceRefresh >= std::chrono::seconds(2)) {
        ANwCreator::EnsureVisible(m_options.activity, m_nativeWindow);
        m_lastSurfaceRefresh = now;
    }

    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2(
        static_cast<float>(m_screenWidth),
        static_cast<float>(m_screenHeight));

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplAndroid_NewFrame(m_screenWidth, m_screenHeight);
    ImGui::NewFrame();
    m_frameActive = true;
    return true;
}

bool AImGui::EndFrame() {
    if (!m_state || !m_frameActive || !m_imguiContext) {
        return false;
    }

    ImGui::SetCurrentContext(m_imguiContext);
    ImGui::Render();
    m_frameActive = false;

    glViewport(0, 0, m_screenWidth, m_screenHeight);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if (!eglSwapBuffers(m_display, m_surface)) {
        AIMGUI_LOGE("Overlay eglSwapBuffers failed: 0x%x", eglGetError());
        return false;
    }
    return true;
}

void AImGui::UnInitEnvironment() {
    const EGLState previousEGL;
    const EGLContext ownedContext = m_context;

    if (m_display != EGL_NO_DISPLAY && m_surface != EGL_NO_SURFACE &&
        m_context != EGL_NO_CONTEXT) {
        eglMakeCurrent(m_display, m_surface, m_surface, m_context);
    }

    if (m_imguiContext) {
        ImGui::SetCurrentContext(m_imguiContext);
        if (m_frameActive) {
            ImGui::EndFrame();
            m_frameActive = false;
        }
        if (m_openGLBackendInitialized) {
            ImGui_ImplOpenGL3_Shutdown();
            m_openGLBackendInitialized = false;
        }
        if (m_androidBackendInitialized) {
            ImGui_ImplAndroid_Shutdown();
            m_androidBackendInitialized = false;
        }
        ImGui::DestroyContext(m_imguiContext);
        m_imguiContext = nullptr;
    }

    if (m_display != EGL_NO_DISPLAY) {
        eglMakeCurrent(m_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if (m_surface != EGL_NO_SURFACE) {
            eglDestroySurface(m_display, m_surface);
            m_surface = EGL_NO_SURFACE;
        }
        if (m_context != EGL_NO_CONTEXT) {
            eglDestroyContext(m_display, m_context);
            m_context = EGL_NO_CONTEXT;
        }
        // Do not eglTerminate(EGL_DEFAULT_DISPLAY): the game owns the same
        // display connection and terminating it can invalidate its context.
        m_display = EGL_NO_DISPLAY;
    }

    if (m_nativeWindow) {
        ANwCreator::Destroy(m_options.activity, m_nativeWindow);
        m_nativeWindow = nullptr;
    }

    m_state = false;
    if (previousEGL.context != ownedContext) {
        previousEGL.Restore();
    }
}

} // namespace android