#pragma once

// ImGui keeps exactly ONE global context ("current context"). The renderer owns
// it: draw.h's Draw() (hooked onto eglSwapBuffers, so it runs on the game's own
// render thread) creates it on the first swap, and CleanupGL() destroys it with
// ImGui::DestroyContext() at shutdown.
//
// The input hooks run on the game's OWN input thread, i.e. not the renderer's,
// and they fire in every window where no context is current:
//   * before the first eglSwapBuffers has created one (game still loading),
//   * after CleanupGL() destroyed it (DestroyContext leaves the current context
//     null).
//
// In those windows ImGui::GetCurrentContext() returns null, and ImGui::GetIO()
// is implemented as `IM_ASSERT(GImGui != nullptr); return GImGui->IO;` -- the
// assert compiles out in release builds (NDEBUG), so it dereferences null and
// the process dies. That is the "tap the screen before the overlay has rendered
// and it crashes" bug.
//
// Rule: every ImGui entry point that is NOT the rendering thread must pass
// nx::imguiInputSafe() (or at least nx::imguiContextAlive()) first. Never cache
// the context pointer -- it can be destroyed between two calls.

#include <atomic>

#include "../imgui.h"

namespace nx {

// True only between a successful context creation and the start of teardown.
// Set on the renderer thread, read by the input thread, hence acquire/release.
inline std::atomic<bool>& imguiReady() {
    static std::atomic<bool> ready{false};
    return ready;
}

// The current context can vanish at any moment, so always re-read the pointer.
inline bool imguiContextAlive() {
    return ImGui::GetCurrentContext() != nullptr;
}

// Guard for input/UI hooks running on a foreign thread.
inline bool imguiInputSafe() {
    return imguiReady().load(std::memory_order_acquire) && imguiContextAlive();
}

// Marks the renderer as gone and stops every foreign-thread touch of ImGui.
inline void imguiBeginTeardown() {
    imguiReady().store(false, std::memory_order_release);
}

} // namespace nx
