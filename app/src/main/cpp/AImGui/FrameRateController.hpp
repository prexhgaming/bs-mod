#pragma once

#include <EGL/egl.h>
#include <android/native_activity.h>

namespace FrameRateController {

// Requests the closest Android display mode to targetFps without touching
// the game's EGL swap interval/frame-pacing policy.
bool Apply(ANativeActivity* activity,
           EGLDisplay gameDisplay,
           bool enabled,
           float targetFps);

} // namespace FrameRateController

