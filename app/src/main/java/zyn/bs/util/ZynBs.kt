package zyn.bs.util

import android.util.Log
import java.io.File

/**
 * The in-game mod menu: `libzyn-bs.so` (native sources in `app/src/main/cpp`).
 *
 * There is exactly **one** load site and it lives in the game's own process:
 * [zyn.bs.view.main.BlackBoxLoader]'s `afterApplicationOnCreate`
 * hook, which the engine runs while the cloned game starts up. Nothing here is
 * ever called from `App.onCreate`, because that runs in the *host* process — the
 * overlay has no business being mapped there (it waits for `libGame.so`, reads
 * the game's own files, and hooks the game's sockets).
 *
 * Nothing crosses JNI:
 *  - no `external fun` declarations, no method calls into the library;
 *  - the library starts itself from `__attribute__((constructor)) void __0__()`
 *    in `main.cpp`, which detaches a worker thread and returns immediately, so
 *    the game's main thread is never blocked by the overlay's setup.
 *
 * [load] therefore only has to map the library once and swallow any failure: a
 * missing or incompatible `.so` must never stop the game from launching.
 */
object ZynBs {

    private const val TAG = "ZynBs"

    /** Name of the shared object built by app/src/main/cpp/CMakeLists.txt. */
    const val NATIVE_LIB = "zyn-bs"

    @Volatile
    private var loaded = false

    @Volatile
    private var loadFailed = false

    /**
     * Loads the menu into the current process. Call it only from the guest
     * lifecycle hook — [isGameProcess] enforces that even if a future caller
     * forgets.
     *
     * @return true when the library is mapped in this process.
     */
    @JvmStatic
    @Synchronized
    fun load(packageName: String, userId: Int): Boolean {
        if (loaded) return true
        if (loadFailed) return false

        if (!isGameProcess(packageName)) {
            Log.w(TAG, "refusing to load in ${currentProcessName()}: not the game process")
            return false
        }

        return try {
            System.loadLibrary(NATIVE_LIB)
            loaded = true
            Log.i(TAG, "lib$NATIVE_LIB.so loaded in $packageName (user $userId)")
            true
        } catch (t: Throwable) {
            loadFailed = true
            Log.e(TAG, "cannot load lib$NATIVE_LIB.so in $packageName", t)
            false
        }
    }

    /**
     * True when this process really is the game (or one of its sub-processes).
     *
     * Checked against `/proc/self/cmdline` rather than a [android.content.Context]:
     * inside the sandbox the engine rewrites app paths and renames processes, and
     * the kernel's view of our own cmdline is the one thing that cannot be
     * redirected. Fails open — if the cmdline is unreadable we prefer a working
     * menu over a silently disabled one.
     */
    private fun isGameProcess(packageName: String): Boolean {
        val process = currentProcessName() ?: return true
        return process == packageName || process.startsWith("$packageName:")
    }

    private fun currentProcessName(): String? = runCatching {
        val bytes = File("/proc/self/cmdline").readBytes()
        val end = bytes.indexOf(0.toByte()).let { if (it < 0) bytes.size else it }
        String(bytes, 0, end).trim().takeIf { it.isNotEmpty() }
    }.getOrNull()
}