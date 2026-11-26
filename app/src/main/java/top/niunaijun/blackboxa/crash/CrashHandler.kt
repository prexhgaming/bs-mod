package top.niunaijun.blackboxa.crash

import android.content.Context
import android.content.Intent
import android.os.Build
import android.os.Process
import android.util.Log
import java.io.File
import java.text.SimpleDateFormat
import java.util.Date
import java.util.Locale

/**
 * Global uncaught-exception trap.
 *
 * Installed from [top.niunaijun.blackboxa.app.App.attachBaseContext] immediately
 * after `super.attachBaseContext` and *before* the first `BlackBoxCore` call, so
 * a failure inside engine init is caught too. Installing it any later would miss
 * exactly the crashes this screen exists for.
 *
 * Nothing is written to disk: the formatted report is handed to
 * [CrashActivity] through the intent's `EXTRA_REPORT` string, which the system
 * marshals for us, and the process is killed afterwards. If the viewer cannot be
 * started the report is simply lost — there is no side channel, by design.
 */
object CrashHandler {

    private const val TAG = "CrashHandler"

    const val EXTRA_REPORT = "top.niunaijun.blackboxa.EXTRA_CRASH_REPORT"

    /**
     * Binder transactions are capped around 1 MB; a stack trace is a few KB, but
     * a runaway cause chain could be larger, so clamp well below the limit.
     */
    private const val MAX_REPORT_CHARS = 200_000

    /** Give AMS time to deliver the intent before this process disappears. */
    private const val KILL_DELAY_MS = 600L

    private const val CRASH_PROCESS_SUFFIX = ":crash"

    @Volatile
    private var installed = false

    /** Application context captured at install time, for the report header. */
    @Volatile
    private var appContext: Context? = null

    fun install(context: Context) {
        if (installed) return
        installed = true

        // Safe here: super.attachBaseContext has already run, so the base is a
        // real Context whose applicationContext is valid.
        appContext = runCatching { context.applicationContext }.getOrNull() ?: context

        val previous = Thread.getDefaultUncaughtExceptionHandler()

        Thread.setDefaultUncaughtExceptionHandler { thread, error ->
            var handled = false
            try {
                handled = show(thread, error)
            } catch (t: Throwable) {
                Log.e(TAG, "Failed to display crash", t)
            }

            if (handled) {
                // Our own screen is coming up; the platform dialog would only be
                // a second, less useful UI on top of it.
                die()
            } else if (previous != null) {
                previous.uncaughtException(thread, error)
            } else {
                die()
            }
        }
    }

    /** True when running inside the crash viewer's own process. */
    fun isCrashProcess(context: Context): Boolean {
        return processName(context).endsWith(CRASH_PROCESS_SUFFIX)
    }

    // ------------------------------------------------------------------ internals

    /** @return true when the viewer activity was started. */
    private fun show(thread: Thread, error: Throwable): Boolean {
        val context = appContext ?: return false

        // The viewer itself crashing must not recurse.
        if (isCrashProcess(context)) return false

        val report = buildReport(thread, error)

        val launched = runCatching {
            val intent = Intent(context, CrashActivity::class.java).apply {
                putExtra(EXTRA_REPORT, report)
                addFlags(
                    Intent.FLAG_ACTIVITY_NEW_TASK or
                        Intent.FLAG_ACTIVITY_CLEAR_TASK or
                        Intent.FLAG_ACTIVITY_NO_ANIMATION or
                        Intent.FLAG_ACTIVITY_EXCLUDE_FROM_RECENTS
                )
            }
            context.startActivity(intent)
        }.isSuccess

        Log.e(TAG, "Crash captured (viewer=$launched)", error)
        return launched
    }

    private fun buildReport(thread: Thread, error: Throwable): String {
        val context = appContext
        val stamp = SimpleDateFormat("yyyy-MM-dd HH:mm:ss.SSS", Locale.US).format(Date())

        val sb = StringBuilder(2048)
        sb.appendLine("BS-KILLER crash report")
        sb.appendLine("time      : $stamp")
        sb.appendLine("process   : ${context?.let { processName(it) } ?: "?"}")
        sb.appendLine("thread    : ${thread.name} (id ${thread.id})")
        sb.appendLine("device    : ${Build.MANUFACTURER} ${Build.MODEL} (${Build.DEVICE})")
        sb.appendLine("android   : ${Build.VERSION.RELEASE} (API ${Build.VERSION.SDK_INT})")
        sb.appendLine("abi       : ${Build.SUPPORTED_ABIS.joinToString()}")
        if (context != null) {
            sb.appendLine("app       : ${versionName(context)} (${versionCode(context)})")
        }
        sb.appendLine()
        sb.appendLine(error.toString())
        sb.appendLine()
        // Full trace, including "Caused by" and suppressed exceptions.
        sb.append(Log.getStackTraceString(error))

        return if (sb.length > MAX_REPORT_CHARS) {
            sb.setLength(MAX_REPORT_CHARS)
            sb.append("\n… report truncated")
            sb.toString()
        } else {
            sb.toString()
        }
    }

    private fun processName(context: Context): String {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.P) {
            runCatching { android.app.Application.getProcessName() }.getOrNull()
                ?.takeIf { it.isNotBlank() }
                ?.let { return it }
        }
        return runCatching {
            // cmdline is NUL separated; only argv[0] matters here.
            File("/proc/self/cmdline").readText().substringBefore('\u0000').trim()
        }.getOrNull()?.takeIf { it.isNotBlank() } ?: context.packageName
    }

    private fun versionName(context: Context): String = runCatching {
        context.packageManager.getPackageInfo(context.packageName, 0).versionName
    }.getOrNull() ?: "?"

    @Suppress("DEPRECATION")
    private fun versionCode(context: Context): Long = runCatching {
        val info = context.packageManager.getPackageInfo(context.packageName, 0)
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.P) {
            info.longVersionCode
        } else {
            info.versionCode.toLong()
        }
    }.getOrNull() ?: -1L

    private fun die() {
        runCatching { Thread.sleep(KILL_DELAY_MS) }
        Process.killProcess(Process.myPid())
        kotlin.system.exitProcess(10)
    }
}
