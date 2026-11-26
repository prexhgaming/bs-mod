package top.niunaijun.blackboxa.crash

import android.content.ClipData
import android.content.ClipboardManager
import android.content.Context
import android.content.Intent
import android.os.Bundle
import android.view.View
import android.widget.TextView
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import top.niunaijun.blackboxa.R

/**
 * Displays the report handed over by [CrashHandler] in the intent extras.
 *
 * Runs in its own `:crash` process (see the manifest) so it survives the death
 * of the process that crashed. The engine is never initialised here —
 * [top.niunaijun.blackboxa.app.App] skips engine setup in this process on
 * purpose — so the screen renders even when the sandbox is what failed.
 */
class CrashActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_crash)

        val logView = findViewById<TextView>(R.id.crashLog)
        val emptyView = findViewById<TextView>(R.id.crashEmpty)
        val summaryView = findViewById<TextView>(R.id.crashSummary)

        // The whole report travels in the intent: no file, no cache, no history.
        val report = intent?.getStringExtra(CrashHandler.EXTRA_REPORT)

        if (report.isNullOrBlank()) {
            logView.visibility = View.GONE
            emptyView.visibility = View.VISIBLE
            summaryView.text = getString(R.string.crash_hint_empty)
        } else {
            logView.text = report
            summaryView.text = getString(R.string.crash_hint)
        }

        findViewById<TextView>(R.id.crashCopy).setOnClickListener {
            if (report.isNullOrBlank()) return@setOnClickListener
            val clipboard = getSystemService(Context.CLIPBOARD_SERVICE) as? ClipboardManager
            if (clipboard == null) {
                Toast.makeText(this, R.string.crash_copy_failed, Toast.LENGTH_SHORT).show()
                return@setOnClickListener
            }
            clipboard.setPrimaryClip(ClipData.newPlainText("crash", report))
            Toast.makeText(this, R.string.crash_copied, Toast.LENGTH_SHORT).show()
        }

        findViewById<TextView>(R.id.crashRestart).setOnClickListener {
            // Cold start of the launcher: engine and sandbox are rebuilt from
            // scratch, which is the only sane recovery after a crash.
            runCatching {
                val restart = Intent()
                    .setClassName(this, LAUNCHER_ACTIVITY)
                    .addFlags(Intent.FLAG_ACTIVITY_NEW_TASK or Intent.FLAG_ACTIVITY_CLEAR_TASK)
                startActivity(restart)
            }
            finish()
        }

        findViewById<TextView>(R.id.crashClose).setOnClickListener { finish() }
    }

    companion object {
        private const val LAUNCHER_ACTIVITY = "top.niunaijun.blackboxa.view.game.GameActivity"
    }
}
