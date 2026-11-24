package top.niunaijun.blackboxa.util

import android.Manifest
import android.content.Context
import android.content.Intent
import android.content.pm.PackageManager
import android.net.Uri
import android.os.Build
import android.os.Environment
import android.provider.Settings
import android.util.Log
import androidx.core.content.ContextCompat
import java.io.File

/**
 * Python SDK dumper (opt-in).
 *
 * The target game statically links CPython into its own `libGame.so`, so the
 * runtime only exists **inside the game process**. `libbs_dumper.so` is therefore
 * loaded into that process while the game starts: its `JNI_OnLoad` spawns a
 * worker thread which discovers the sandbox layout from `/proc/self/maps` and
 * writes the dump there.
 *
 * Native needs exactly one thing from Java — the public Downloads path, which
 * only the framework can resolve — and that is what [nativeStart] hands over.
 * Everything else is resolved natively, in the process that owns the runtime.
 *
 * The toggle lives in the launcher's checkable "More" menu item and is persisted,
 * so flipping it takes effect on the **next** game launch.
 */
object PySdkDumper {

    private const val TAG = "PySdkDumper"
    private const val PREFS = "bs_dumper_prefs"
    private const val KEY_ENABLED = "sdk_dumper_enabled"
    private const val NATIVE_LIB = "bs_dumper"
    private const val DUMP_DIR_NAME = "sdk"

    @Volatile private var loaded = false
    @Volatile private var loadFailed = false

    /**
     * Hands native the one path Java alone can resolve. Native arms its dump
     * thread with it; all other work happens on a thread inside the library.
     */
    private external fun nativeStart(outputDir: String)

    // ------------------------------------------------------------------- prefs

    fun isEnabled(context: Context): Boolean =
        prefs(context).getBoolean(KEY_ENABLED, false)

    fun setEnabled(context: Context, enabled: Boolean) {
        prefs(context).edit().putBoolean(KEY_ENABLED, enabled).apply()
        Log.i(TAG, "dumper ${if (enabled) "enabled" else "disabled"}")
    }

    private fun prefs(context: Context) =
        context.getSharedPreferences(PREFS, Context.MODE_PRIVATE)

    // ------------------------------------------------------------------ output

    /**
     * `/storage/emulated/0/Download/<game-package>/sdk`.
     *
     * The package is passed in instead of read from the context so the caller
     * decides which app the dump is attributed to.
     */
    fun outputDir(context: Context, packageName: String): File {
        val downloads =
            Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DOWNLOADS)
        return File(File(downloads, packageName), DUMP_DIR_NAME)
    }

    /**
     * True when the dump may write into the public Downloads folder.
     *
     * API <= 29: `WRITE_EXTERNAL_STORAGE` (legacy external storage allows raw
     * paths). API >= 30: "All files access" (`MANAGE_EXTERNAL_STORAGE`), because
     * scoped storage rejects raw writes into `Download/` without it.
     */
    @JvmStatic
    fun hasExternalWriteAccess(context: Context): Boolean = when {
        Build.VERSION.SDK_INT >= Build.VERSION_CODES.R ->
            Environment.isExternalStorageManager()

        else -> ContextCompat.checkSelfPermission(
            context, Manifest.permission.WRITE_EXTERNAL_STORAGE
        ) == PackageManager.PERMISSION_GRANTED
    }

    /**
     * Settings screen granting "All files access", or null below API 30.
     *
     * This permission has no runtime dialog — the user has to flip it manually,
     * so we deep-link straight to this app's entry in that list.
     */
    @JvmStatic
    fun allFilesAccessIntent(context: Context): Intent? {
        if (Build.VERSION.SDK_INT < Build.VERSION_CODES.R) return null
        return Intent(Settings.ACTION_MANAGE_APP_ALL_FILES_ACCESS_PERMISSION)
            .setData(Uri.fromParts("package", context.packageName, null))
    }

    /**
     * Directory the dump is really written to.
     *
     * A raw path into the public Downloads folder needs "All files access" on
     * API 30+; without it we fall back to the app's own external files dir, which
     * is always writable. Returns null if nothing is writable.
     */
    fun writableOutputDir(context: Context, packageName: String): File? {
        val candidates = listOfNotNull(
            outputDir(context, packageName).takeIf { hasExternalWriteAccess(context) },
            context.getExternalFilesDir(null)?.let { File(it, DUMP_DIR_NAME) },
            File(context.filesDir, DUMP_DIR_NAME)
        )
        for (dir in candidates) {
            if (isWritable(dir)) return dir
        }
        Log.e(TAG, "no writable output directory among $candidates")
        return null
    }

    /** mkdirs + a real write probe, so native never fails on a read-only path. */
    private fun isWritable(dir: File): Boolean {
        if (!dir.exists() && !dir.mkdirs()) {
            Log.w(TAG, "cannot create ${dir.absolutePath}")
            return false
        }
        return try {
            val probe = File(dir, ".probe")
            val created = probe.createNewFile()
            probe.delete()
            created || dir.canWrite()
        } catch (t: Throwable) {
            Log.w(TAG, "not writable: ${dir.absolutePath} (${t.message})")
            false
        }
    }

    // ------------------------------------------------------------------ arming

    /**
     * Called from the guest process once the game's `Application` exists: loads
     * the native library (whose `JNI_OnLoad` spawns the dump thread) and hands it
     * the Downloads folder. Returns false when disabled or unavailable. Never
     * throws — a missing lib must not break the game launch.
     */
    @JvmStatic
    fun arm(context: Context, packageName: String): Boolean {
        if (!isEnabled(context)) {
            Log.i(TAG, "arm: disabled, nothing to do")
            return false
        }
        if (!ensureLoaded()) return false

        val dir = writableOutputDir(context, packageName) ?: run {
            Log.e(TAG, "arm: no writable output dir for $packageName")
            return false
        }
        return try {
            nativeStart(dir.absolutePath)
            Log.i(TAG, "arm: dump armed for $packageName -> ${dir.absolutePath}")
            true
        } catch (t: Throwable) {
            Log.e(TAG, "arm failed", t)
            false
        }
    }

    /** Loads `libbs_dumper.so`. Safe to call repeatedly; never throws. */
    @Synchronized
    fun ensureLoaded(): Boolean {
        if (loaded) return true
        if (loadFailed) return false
        return try {
            System.loadLibrary(NATIVE_LIB)
            loaded = true
            Log.i(TAG, "loaded lib$NATIVE_LIB.so")
            true
        } catch (t: Throwable) {
            loadFailed = true
            Log.e(TAG, "lib$NATIVE_LIB.so unavailable: ${t.message}")
            false
        }
    }
}
