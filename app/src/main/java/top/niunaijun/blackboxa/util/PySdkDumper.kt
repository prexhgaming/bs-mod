package top.niunaijun.blackboxa.util

import android.content.Context
import android.os.Environment
import android.util.Log
import java.io.File

/**
 * Python SDK dumper.
 *
 * The target game is a CPython title: its interpreter is statically linked into
 * the game's own `libGame.so`, so the runtime only exists **inside the game
 * process**. We therefore ship a small native library (`libbs_dumper.so`, built
 * from `app/src/main/cpp/sdk_dumper.cpp`) that is loaded into that process while
 * the game starts; it resolves the CPython entry points the game already has and
 * serializes the real runtime layout into a portable SDK folder.
 *
 * Output: `/storage/emulated/0/Download/<app-package>/sdk`
 *
 * The dumper is opt-in. It is toggled with the checkable "Python SDK dumper" item
 * in the launcher's More menu and is persisted in shared preferences, so the next
 * game process start performs the dump.
 *
 * Threading: the native calls block while copying module trees, so [detect],
 * [version] and [dump] are blocking and must be called off the main thread.
 */
object PySdkDumper {

    private const val TAG = "PySdkDumper"
    private const val PREFS = "bs_dumper_prefs"
    private const val KEY_ENABLED = "sdk_dumper_enabled"
    private const val NATIVE_LIB = "bs_dumper"

    /** Why a dump attempt finished the way it did — mapped to strings in the UI. */
    enum class Code { OK, DISABLED, NATIVE_MISSING, NO_RUNTIME, ALREADY_RUNNING, IO_ERROR }

    data class Result(
        val code: Code,
        val outputDir: File? = null,
        val pythonVersion: String? = null,
        val detail: String? = null
    ) {
        val ok: Boolean get() = code == Code.OK
    }

    @Volatile private var loaded = false
    @Volatile private var loadFailed = false
    @Volatile private var running = false

    // ------------------------------------------------------------------ native

    private external fun nativeDetect(nativeLibDir: String, dataDir: String, sourceDir: String): Boolean
    private external fun nativeVersion(): String
    private external fun nativeDump(outDir: String, nativeLibDir: String, dataDir: String, sourceDir: String): String

    /** Loads `libbs_dumper.so`. Safe to call repeatedly; never throws. */
    @Synchronized
    fun ensureLoaded(): Boolean {
        if (loaded) return true
        if (loadFailed) return false
        return try {
            System.loadLibrary(NATIVE_LIB)
            loaded = true
            Log.i(TAG, "libbs_dumper.so loaded")
            true
        } catch (t: Throwable) {
            loadFailed = true
            Log.w(TAG, "libbs_dumper.so unavailable: ${t.message}")
            false
        }
    }

    // ------------------------------------------------------------------- toggle

    private fun prefs(context: Context) =
        context.applicationContext.getSharedPreferences(PREFS, Context.MODE_PRIVATE)

    fun isEnabled(context: Context): Boolean = prefs(context).getBoolean(KEY_ENABLED, false)

    fun setEnabled(context: Context, enabled: Boolean) {
        prefs(context).edit().putBoolean(KEY_ENABLED, enabled).apply()
    }

    // -------------------------------------------------------------------- paths

    /**
     * `/storage/emulated/0/Download/<app-package>/sdk`.
     *
     * Called from inside the game process, [Context.getPackageName] is patched by
     * the sandbox to the game's package, which is what we want to name the folder.
     */
    fun outputDir(context: Context): File {
        val downloads = Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DOWNLOADS)
        return File(File(downloads, context.packageName), "sdk")
    }

    // ------------------------------------------------------------------ actions

    /** True when a Python runtime is visible from this process. Blocking. */
    fun detect(context: Context, nativeLibDir: String, dataDir: String, sourceDir: String): Boolean {
        if (!ensureLoaded()) return false
        return try {
            nativeDetect(nativeLibDir, dataDir, sourceDir)
        } catch (t: Throwable) {
            Log.e(TAG, "detect failed: ${t.message}")
            false
        }
    }

    /** Resolved CPython version string, or null when nothing is visible. Blocking. */
    fun version(): String? {
        if (!ensureLoaded()) return null
        return try {
            nativeVersion().takeIf { it.isNotBlank() }
        } catch (t: Throwable) {
            Log.e(TAG, "version failed: ${t.message}")
            null
        }
    }

    /**
     * Detects the runtime and writes the SDK folder. Blocking — call from a
     * background thread.
     */
    /**
     * One-shot entry point used by the launcher's "More" menu.
     *
     * Returns `"OK|<dir>|<pythonVersion>"` or `"FAIL|<code>|<detail>"`. The
     * detail string of the no-runtime case deliberately contains
     * "no python runtime" — the activity matches on it to pick its message.
     */
    @JvmStatic
    fun run(
        context: Context,
        pkg: String,
        nativeLibDir: String,
        dataDir: String,
        sourceDir: String
    ): String {
        if (!ensureLoaded()) {
            return "FAIL|native|libbs_dumper.so could not be loaded"
        }
        if (running) {
            return "FAIL|busy|a dump is already running"
        }
        running = true
        return try {
            if (!detect(context, nativeLibDir, dataDir, sourceDir)) {
                return "FAIL|nopy|no python runtime found in $nativeLibDir"
            }
            val dir = File(
                File(
                    Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DOWNLOADS),
                    pkg
                ),
                "sdk"
            )
            if (!dir.exists() && !dir.mkdirs()) {
                return "FAIL|io|cannot create ${dir.absolutePath}"
            }
            val written = nativeDump(
                dir.absolutePath,
                nativeLibDir,
                dataDir,
                sourceDir
            )
            if (written.isNullOrBlank()) {
                "FAIL|io|native dumper produced nothing"
            } else {
                "OK|${dir.absolutePath}|${version().orEmpty()}"
            }
        } catch (t: Throwable) {
            Log.e(TAG, "sdk dump failed", t)
            "FAIL|exception|${t.message ?: t.javaClass.simpleName}"
        } finally {
            running = false
        }
    }

    fun dump(context: Context, nativeLibDir: String, dataDir: String, sourceDir: String): Result {
        if (!ensureLoaded()) return Result(Code.NATIVE_MISSING)
        if (running) return Result(Code.ALREADY_RUNNING)
        running = true
        return try {
            if (!detect(context, nativeLibDir, dataDir, sourceDir)) {
                return Result(Code.NO_RUNTIME)
            }

            val outDir = outputDir(context)
            if (!outDir.exists() && !outDir.mkdirs()) {
                return Result(Code.IO_ERROR, detail = outDir.absolutePath)
            }

            val written = nativeDump(
                outDir.absolutePath, nativeLibDir, dataDir, sourceDir
            )
            if (written.isNullOrBlank()) {
                Result(Code.IO_ERROR, outputDir = outDir, detail = outDir.absolutePath)
            } else {
                Result(Code.OK, outputDir = File(written), pythonVersion = version())
            }
        } catch (t: Throwable) {
            Log.e(TAG, "dump failed: ${t.message}")
            Result(Code.IO_ERROR, detail = t.message)
        } finally {
            running = false
        }
    }

    /**
     * Hook for `App.onCreate`: dumps automatically when the user enabled it and we
     * are running inside the game process. Uses the current process' own
     * `ApplicationInfo`, which the sandbox rewrites to the game's, so no engine
     * internals are needed. Runs on a detached background thread and never blocks
     * or throws.
     */
    fun autoDumpOnStart(context: Context) {
        if (!isEnabled(context)) return
        if (!ensureLoaded()) return

        val appInfo = try {
            context.applicationInfo
        } catch (t: Throwable) {
            Log.w(TAG, "autoDump: no application info: ${t.message}")
            return
        }

        val nativeLibDir = appInfo.nativeLibraryDir ?: return
        val dataDir = appInfo.dataDir ?: return
        val sourceDir = appInfo.sourceDir ?: return

        Thread({
            val result = dump(context, nativeLibDir, dataDir, sourceDir)
            Log.i(TAG, "autoDump: ${result.code} ${result.outputDir ?: ""}")
        }, "bs-sdk-dumper").apply { isDaemon = true }.start()
    }
}
