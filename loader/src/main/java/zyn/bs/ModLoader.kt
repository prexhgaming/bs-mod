package zyn.bs

import android.content.Context
import android.util.Log
import com.fesu.renjana.core.GuestModInjection

/**
 * Loads the injected mod into the guest process.
 *
 * This lives in `:loader`, not in `:core`, on purpose: the library name and the
 * target package are app decisions, and keeping them here means someone forking
 * this project changes [GameTarget.MOD_LIBRARY] / [GameTarget.PACKAGE_NAME] and
 * nothing else. The engine only knows it can hand a guest process to whoever
 * registered through [GuestModInjection].
 *
 * The `.so` is built from `loader/src/main/cpp` by `externalNativeBuild`, so it
 * is already in this APK's `nativeLibraryDir` — `System.loadLibrary` finds it
 * with no path handling.
 */
object ModLoader : GuestModInjection.Installer {

    private const val TAG = "ModLoader"

    /**
     * Per-process guard. dlopen is idempotent, but this keeps the log honest
     * about how many times we actually tried.
     */
    @Volatile
    private var loaded = false

    /** Hands this installer to the engine. Called once from [LoaderApp.onCreate]. */
    fun register() {
        GuestModInjection.register(this)
    }

    override fun install(context: Context, guestPackageName: String) {
        if (loaded) return
        // Never load a Knives Out-targeted hook set into an unrelated guest.
        if (guestPackageName != GameTarget.PACKAGE_NAME) return
        loaded = true

        try {
            System.loadLibrary(GameTarget.MOD_LIBRARY)
            Log.i(TAG, "lib${GameTarget.MOD_LIBRARY}.so loaded into $guestPackageName")
        } catch (t: Throwable) {
            // Most common cause: the APK was built without the armeabi-v7a .so
            // (see abiFilters in build.gradle) or the device is arm64-only.
            Log.e(TAG, "lib${GameTarget.MOD_LIBRARY}.so load failed for $guestPackageName", t)
        }
    }
}
