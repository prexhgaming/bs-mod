package top.niunaijun.blackboxa.util

import android.content.Context
import android.util.Log
import java.io.File

/**
 * Python SDK dumper (opt-in).
 *
 * The target game statically links CPython into its own `libGame.so`, so the
 * interpreter only exists **inside the game's process**. The work therefore has
 * to happen there, but the *decision* belongs to the launcher:
 *
 *  1. The user flips the toggle in the "More" menu -> persisted in prefs.
 *  2. Before starting the game, the launcher calls [prepareLaunch], which
 *     leaves an arm-request file in the game's sandbox.
 *  3. The guest lifecycle hook calls [loadIfArmed] inside the game's process;
 *     only then is `libbs_dumper.so` loaded.
 *  4. A `__attribute__((constructor))` in that library fires the moment the
 *     dynamic linker loads it, detaches a worker thread and returns, so the
 *     game's main thread never blocks. The worker discovers everything itself
 *     (package, native-lib dir, sandbox layout) from `/proc/self/maps` and
 *     writes to `/storage/emulated/0/Android/data/<game-pkg>/sdk`.
 *
 * There are deliberately no `external fun` declarations here and no `JNI_OnLoad`
 * in the library: the native side needs nothing from Java, so nothing crosses
 * the JNI boundary in either direction.
 */
object PySdkDumper {

    private const val TAG = "PySdkDumper"

    private const val PREFS = "bs_dumper_prefs"
    private const val KEY_ENABLED = "sdk_dumper_enabled"
    private const val NATIVE_LIB = "bs_dumper"

    /** Written by the launcher, read by the guest process. */
    private const val ARM_FILE = "sdk_dumper.arm"

    /** Our own applicationId; the engine keeps the sandbox under its data dir. */
    private const val HOST_PACKAGE = "top.niunaijun.blackboxa"

    @Volatile
    private var loaded = false

    @Volatile
    private var loadFailed = false

    // ------------------------------------------------------------ toggle

    private fun prefs(context: Context) =
        context.getSharedPreferences(PREFS, Context.MODE_PRIVATE)

    fun isEnabled(context: Context): Boolean =
        runCatching { prefs(context).getBoolean(KEY_ENABLED, false) }.getOrDefault(false)

    fun setEnabled(context: Context, enabled: Boolean) {
        runCatching {
            prefs(context).edit().putBoolean(KEY_ENABLED, enabled).apply()
        }.onFailure { Log.e(TAG, "cannot persist toggle", it) }
        Log.i(TAG, "toggle -> $enabled")
    }

    // ------------------------------------------------------------ arm request

    /**
     * Absolute path of the arm request.
     *
     * Built from constants rather than from a [Context] on purpose: inside the
     * guest process the engine redirects app paths into the sandbox, so a
     * Context-derived path would resolve somewhere else than on the host side.
     * The file lives in the game's *virtual* data dir, which the engine creates
     * and both processes can reach by this exact real path.
     */
    private fun armFile(packageName: String, userId: Int): File =
        File(
            "/data/user/0/$HOST_PACKAGE/blackbox/data/user/$userId/" +
                "$packageName/files/$ARM_FILE"
        )

    /**
     * Host side. Called right before the game is started, so the guest process
     * always sees a decision that matches the current checkbox state.
     *
     * @return true when the dumper is armed for this launch.
     */
    @JvmStatic
    fun prepareLaunch(
        context: Context,
        packageName: String,
        userId: Int
    ): Boolean {
        val file = armFile(packageName, userId)
        if (!isEnabled(context)) {
            if (file.exists() && !file.delete()) {
                Log.w(TAG, "prepareLaunch: cannot remove ${file.absolutePath}")
            }
            Log.i(TAG, "prepareLaunch: disabled, dumper stays off")
            return false
        }

        return try {
            file.parentFile?.mkdirs()
            file.writeText("armed ${System.currentTimeMillis()}\n")
            Log.i(TAG, "prepareLaunch: armed -> ${file.absolutePath}")
            true
        } catch (t: Throwable) {
            Log.e(TAG, "prepareLaunch: cannot write ${file.absolutePath}", t)
            false
        }
    }

    /** True when the launcher armed the dumper for the current game launch. */
    @JvmStatic
    fun isArmed(packageName: String, userId: Int): Boolean =
        runCatching { armFile(packageName, userId).isFile }.getOrDefault(false)

    /** Drops the arm request once it has been consumed. */
    @JvmStatic
    fun consumeArm(packageName: String, userId: Int) {
        runCatching {
            val file = armFile(packageName, userId)
            if (file.exists() && !file.delete()) {
                Log.w(TAG, "consumeArm: cannot delete ${file.absolutePath}")
            }
        }
    }

    // ------------------------------------------------------------ guest side

    /**
     * Guest side, called from the game's `Application.onCreate`. Loads the
     * native library only when the launcher armed this launch. Never throws —
     * a missing or failing library must not break the game.
     */
    @JvmStatic
    fun loadIfArmed(packageName: String, userId: Int): Boolean {
        if (!isArmed(packageName, userId)) {
            Log.i(TAG, "load: not armed by the launcher, skipping")
            return false
        }
        val ok = ensureLoaded()
        if (ok) {
            Log.i(TAG, "load: lib$NATIVE_LIB.so loaded, worker owns the dump")
            consumeArm(packageName, userId)
        }
        return ok
    }

    @Synchronized
    private fun ensureLoaded(): Boolean {
        if (loaded) return true
        if (loadFailed) return false
        return try {
            System.loadLibrary(NATIVE_LIB)
            loaded = true
            true
        } catch (t: Throwable) {
            loadFailed = true
            Log.e(TAG, "cannot load lib$NATIVE_LIB.so", t)
            false
        }
    }
}
