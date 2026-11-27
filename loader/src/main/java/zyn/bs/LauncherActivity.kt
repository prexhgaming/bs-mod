package zyn.bs

import android.animation.ObjectAnimator
import android.animation.ValueAnimator
import android.content.res.ColorStateList
import android.graphics.Bitmap
import android.graphics.BitmapFactory
import android.net.Uri
import android.os.Bundle
import android.view.View
import android.view.animation.DecelerateInterpolator
import android.widget.Toast
import androidx.activity.result.contract.ActivityResultContracts
import androidx.appcompat.app.AlertDialog
import androidx.appcompat.app.AppCompatActivity
import androidx.appcompat.widget.PopupMenu
import androidx.core.content.ContextCompat
import androidx.core.view.ViewCompat
import androidx.core.view.WindowCompat
import androidx.core.view.WindowInsetsCompat
import androidx.core.view.updatePadding
import androidx.lifecycle.lifecycleScope
import com.fesu.renjana.RenjanaApplication
import com.fesu.renjana.core.LaunchResult
import com.fesu.renjana.models.Instance
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.flow.first
import kotlinx.coroutines.launch
import kotlinx.coroutines.withContext
import zyn.bs.databinding.ActivityLauncherBinding
import java.io.File

/**
 * The whole app: one game, one status pill, one action.
 *
 * The container itself is `:core` (renjana). This screen only decides *whether*
 * the game can run — it resolves the target from the package manager, makes sure
 * exactly one container instance exists for it, and hands off to
 * [com.fesu.renjana.core.InstanceLauncher]. It never touches stub activities,
 * hooks or virtual storage directly.
 */
class LauncherActivity : AppCompatActivity() {

    private lateinit var binding: ActivityLauncherBinding

    private val engine: RenjanaApplication get() = RenjanaApplication.get()

    private var instance: Instance? = null
    private var state: LauncherState = LauncherState.WAITING
    private var busy = false
    private var refreshing = false
    private var noticePicksApk = false

    private var glowAnimator: ObjectAnimator? = null
    private var iconGlowAnimator: ObjectAnimator? = null
    private var shineAnimator: ObjectAnimator? = null

    /** Re-armed after every sweep, so the CTA keeps a slow travelling highlight. */
    private val shineLoop = object : Runnable {
        override fun run() {
            if (isFinishing || isDestroyed) return
            sweepShine()
            binding.launchButton.postDelayed(this, SHINE_PERIOD_MS)
        }
    }

    private val pickApk = registerForActivityResult(ActivityResultContracts.OpenDocument()) { uri ->
        if (uri != null) importApk(uri)
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityLauncherBinding.inflate(layoutInflater)
        setContentView(binding.root)

        WindowCompat.setDecorFitsSystemWindows(window, false)

        binding.footer.text = getString(R.string.launcher_footer, versionName())

        applyInsets()
        loadWallpaper()
        bindTarget()
        bindActions()
        playIntro()
    }

    override fun onResume() {
        super.onResume()
        // Also covers coming back from the store: installing the game there has
        // to flip MISSING to READY without restarting the launcher.
        refresh()
    }

    override fun onDestroy() {
        binding.launchButton.removeCallbacks(shineLoop)
        glowAnimator?.cancel()
        iconGlowAnimator?.cancel()
        shineAnimator?.cancel()
        super.onDestroy()
    }

    // ── Layout ────────────────────────────────────────────────────────────────

    /** Edge-to-edge: the artwork runs under both bars, content respects them. */
    private fun applyInsets() {
        val gutter = resources.getDimensionPixelSize(R.dimen.launcher_gutter)
        val topGap = resources.getDimensionPixelSize(R.dimen.launcher_top_gap)
        val bottomGap = resources.getDimensionPixelSize(R.dimen.launcher_bottom_gap)

        ViewCompat.setOnApplyWindowInsetsListener(binding.content) { view, insets ->
            val bars = insets.getInsets(WindowInsetsCompat.Type.systemBars())
            view.updatePadding(
                left = gutter + bars.left,
                top = topGap + bars.top,
                right = gutter + bars.right,
                bottom = bottomGap + bars.bottom
            )
            insets
        }
    }

    /** Decodes the wallpaper off the main thread and fades it in over the base. */
    private fun loadWallpaper() {
        lifecycleScope.launch {
            val bitmap = withContext(Dispatchers.IO) { decodeWallpaper() } ?: return@launch
            binding.heroImage.setImageBitmap(bitmap)
            binding.heroImage.alpha = 0f
            binding.heroImage.animate().alpha(1f).setDuration(WALLPAPER_FADE_MS).start()
        }
    }

    /**
     * The asset is 1472×2590 — decoding it at full size as ARGB_8888 is ~15 MB.
     * Downsample to roughly the screen size and use RGB_565 (a wallpaper never
     * needs an alpha channel), which keeps it under ~2 MB.
     */
    private fun decodeWallpaper(): Bitmap? = runCatching {
        val bounds = BitmapFactory.Options().apply { inJustDecodeBounds = true }
        assets.open(WALLPAPER_ASSET).use { BitmapFactory.decodeStream(it, null, bounds) }

        val metrics = resources.displayMetrics
        var sample = 1
        while (bounds.outWidth / (sample * 2) >= metrics.widthPixels &&
            bounds.outHeight / (sample * 2) >= metrics.heightPixels
        ) {
            sample *= 2
        }

        val options = BitmapFactory.Options().apply {
            inSampleSize = sample
            inPreferredConfig = Bitmap.Config.RGB_565
        }
        assets.open(WALLPAPER_ASSET).use { BitmapFactory.decodeStream(it, null, options) }
    }.getOrNull()

    private fun bindTarget() {
        val target = GameTarget.resolve(this)
        binding.gameTitle.text = target?.label ?: getString(R.string.app_name)

        val icon = GameTarget.icon(this)
        if (icon != null) {
            binding.gameIcon.setImageDrawable(icon)
            binding.gameIcon.visibility = View.VISIBLE
            binding.iconMonogram.visibility = View.GONE
        } else {
            // No game yet: the monogram holds the circle so the layout never
            // collapses while the user decides to install or import.
            binding.gameIcon.visibility = View.GONE
            binding.iconMonogram.visibility = View.VISIBLE
        }
    }

    private fun bindActions() {
        binding.launchButton.setOnClickListener { onPrimaryAction() }
        binding.noticeAction.setOnClickListener {
            if (noticePicksApk) pickApk.launch(arrayOf(APK_MIME)) else refresh()
        }
        binding.moreButton.setOnClickListener { showMore(it) }
    }

    // ── Motion ────────────────────────────────────────────────────────────────

    private fun playIntro() {
        val rise = resources.getDimensionPixelSize(R.dimen.launcher_rise).toFloat()
        binding.content.translationY = rise
        binding.content.alpha = 0f
        binding.content.animate()
            .translationY(0f)
            .alpha(1f)
            .setDuration(INTRO_DURATION_MS)
            .setInterpolator(DecelerateInterpolator())
            .start()

        glowAnimator = pulse(binding.ctaGlow, GLOW_PERIOD_MS)
        iconGlowAnimator = pulse(binding.iconGlow, GLOW_PERIOD_MS + 400L)

        binding.launchButton.postDelayed(shineLoop, SHINE_RETRY_MS)
    }

    /** Alpha-only breathing: no layout passes, so it stays on the render thread. */
    private fun pulse(view: View, duration: Long): ObjectAnimator =
        ObjectAnimator.ofFloat(view, View.ALPHA, GLOW_MIN, GLOW_MAX).apply {
            this.duration = duration
            repeatMode = ValueAnimator.REVERSE
            repeatCount = ValueAnimator.INFINITE
            start()
        }

    private fun sweepShine() {
        val travel = binding.launchButton.width.toFloat()
        if (travel <= 0f) return
        val start = -binding.buttonShine.width.toFloat()

        shineAnimator?.cancel()
        binding.buttonShine.translationX = start
        shineAnimator = ObjectAnimator.ofFloat(
            binding.buttonShine,
            View.TRANSLATION_X,
            start,
            travel
        ).apply {
            duration = SHINE_SWEEP_MS
            interpolator = DecelerateInterpolator()
            start()
        }
    }

    // ── Provisioning ──────────────────────────────────────────────────────────

    /**
     * Resolves the target and guarantees one instance exists for it.
     *
     * Idempotent: an existing instance for the same package is reused, so
     * repeated [onResume] calls never stack containers or reset game data.
     */
    private fun refresh() {
        if (busy || refreshing) return
        refreshing = true

        lifecycleScope.launch {
            try {
                render(LauncherState.WAITING)

                val target = GameTarget.resolve(this@LauncherActivity)
                if (target == null) {
                    instance = null
                    render(LauncherState.MISSING)
                    return@launch
                }

                render(LauncherState.PREPARING)
                val manager = engine.instanceManager
                val existing = withContext(Dispatchers.IO) {
                    manager.getAllInstances().first()
                }.firstOrNull { it.packageName == target.packageName }

                instance = existing ?: withContext(Dispatchers.IO) {
                    manager.createInstance(
                        packageName = target.packageName,
                        appName = target.label,
                        versionName = target.versionName,
                        versionCode = target.versionCode,
                        apkPath = target.apkPath,
                        accountId = null
                    ).getOrNull()
                }

                render(if (instance != null) LauncherState.READY else LauncherState.MISSING)
            } finally {
                refreshing = false
            }
        }
    }

    private fun onPrimaryAction() {
        if (busy) return
        if (state == LauncherState.MISSING) {
            pickApk.launch(arrayOf(APK_MIME))
            return
        }
        launchGame()
    }

    private fun launchGame() {
        val current = instance
        if (current == null) {
            refresh()
            return
        }
        busy = true
        binding.launchButton.isEnabled = false
        binding.launchLabel.setText(R.string.action_preparing)

        lifecycleScope.launch {
            val result = runCatching {
                engine.instanceLauncher.launchApp(current.id, current.packageName)
            }.getOrNull()

            busy = false
            binding.launchButton.isEnabled = true

            when (result) {
                LaunchResult.Success -> render(LauncherState.READY)
                is LaunchResult.FallbackNoIsolation -> {
                    toast(getString(R.string.toast_launch_failed))
                    render(LauncherState.READY)
                }
                is LaunchResult.Failure -> showError(result.message)
                null -> showError(null)
            }
        }
    }

    /**
     * Manual import: the game is not installed on the device, so the user picks
     * its APK. We copy it into internal storage first — the container needs a
     * stable path, and a content:// URI can lose its grant after a process death.
     */
    private fun importApk(uri: Uri) {
        if (busy) return
        busy = true

        lifecycleScope.launch {
            render(LauncherState.PREPARING)

            val copied = withContext(Dispatchers.IO) { copyApk(uri) }
            if (copied == null) {
                busy = false
                showError(getString(R.string.message_import_failed))
                return@launch
            }

            val resolved = GameTarget.readArchive(this@LauncherActivity, copied.absolutePath)
            if (resolved == null) {
                busy = false
                showError(getString(R.string.message_import_failed))
                return@launch
            }

            val created = withContext(Dispatchers.IO) {
                engine.instanceManager.createInstance(
                    packageName = resolved.packageName,
                    appName = resolved.label,
                    versionName = resolved.versionName,
                    versionCode = resolved.versionCode,
                    apkPath = resolved.apkPath,
                    accountId = null
                ).getOrNull()
            }

            instance = created
            busy = false
            binding.gameTitle.text = resolved.label
            render(if (created != null) LauncherState.READY else LauncherState.MISSING)
            if (created == null) showError(getString(R.string.message_import_failed))
        }
    }

    private fun copyApk(uri: Uri): File? = runCatching {
        val stream = contentResolver.openInputStream(uri) ?: return null
        val dir = File(filesDir, "imports").apply { mkdirs() }
        val out = File(dir, "game.apk")
        stream.use { input -> out.outputStream().use { input.copyTo(it) } }
        out
    }.getOrNull()

    // ── Rendering ─────────────────────────────────────────────────────────────

    private fun render(next: LauncherState) {
        state = next
        when (next) {
            LauncherState.READY -> {
                pill(
                    R.string.launcher_status_ready,
                    R.color.launcher_pill_ready_bg,
                    R.color.launcher_pill_ready_text
                )
                binding.launchLabel.setText(R.string.action_launch)
                binding.launchIcon.visibility = View.VISIBLE
                binding.launchHint.setText(R.string.launcher_hint_ready)
                binding.preparingBar.visibility = View.GONE
                binding.noticeCard.visibility = View.GONE
                dial(1f)
            }

            LauncherState.PREPARING, LauncherState.WAITING -> {
                pill(
                    R.string.launcher_status_preparing,
                    R.color.launcher_pill_neutral_bg,
                    R.color.launcher_pill_neutral_text
                )
                binding.launchLabel.setText(R.string.action_preparing)
                binding.launchIcon.visibility = View.GONE
                binding.launchHint.setText(
                    if (next == LauncherState.WAITING) {
                        R.string.launcher_stage_waiting
                    } else {
                        R.string.launcher_hint_preparing
                    }
                )
                binding.preparingBar.visibility = View.VISIBLE
                binding.noticeCard.visibility = View.GONE
                dial(0.85f)
            }

            LauncherState.MISSING -> {
                pill(
                    R.string.launcher_status_missing,
                    R.color.launcher_pill_warn_bg,
                    R.color.launcher_pill_warn_text
                )
                binding.launchLabel.setText(R.string.action_install)
                binding.launchIcon.visibility = View.VISIBLE
                binding.launchHint.text = getString(
                    R.string.launcher_hint_missing,
                    GameTarget.resolve(this)?.label ?: GameTarget.PACKAGE_NAME
                )
                binding.preparingBar.visibility = View.GONE
                showNotice(
                    title = getString(R.string.notice_missing_title, GameTarget.PACKAGE_NAME),
                    body = getString(R.string.notice_missing_body),
                    action = getString(R.string.action_select_apk),
                    picksApk = true
                )
                dial(0.78f)
            }
        }
    }

    private fun showNotice(title: String, body: String, action: String, picksApk: Boolean) {
        noticePicksApk = picksApk
        binding.noticeCard.visibility = View.VISIBLE
        binding.noticeTitle.text = title
        binding.noticeBody.text = body
        binding.noticeAction.text = action
    }

    private fun showError(message: String?) {
        render(LauncherState.READY)
        showNotice(
            title = getString(R.string.notice_error_title),
            body = message ?: getString(R.string.toast_launch_failed),
            action = getString(R.string.action_retry),
            picksApk = false
        )
    }

    private fun pill(textRes: Int, backgroundColor: Int, textColor: Int) {
        binding.statusPill.setText(textRes)
        binding.statusPill.backgroundTintList =
            ColorStateList.valueOf(ContextCompat.getColor(this, backgroundColor))
        binding.statusPill.setTextColor(ContextCompat.getColor(this, textColor))
    }

    /** Dims the CTA when the sandbox is not ready, so "not yet" reads at a glance. */
    private fun dial(alpha: Float) {
        binding.launchButton.alpha = alpha
    }

    // ── Menu ──────────────────────────────────────────────────────────────────

    private fun showMore(anchor: View) {
        val popup = PopupMenu(this, anchor)
        popup.menu.add(0, MENU_REFRESH, 0, R.string.action_refresh)
        popup.menu.add(0, MENU_CLEAR, 1, R.string.action_clear_data)
        popup.setOnMenuItemClickListener { item ->
            when (item.itemId) {
                MENU_REFRESH -> {
                    refresh()
                    true
                }
                MENU_CLEAR -> {
                    confirmClearData()
                    true
                }
                else -> false
            }
        }
        popup.show()
    }

    private fun confirmClearData() {
        val current = instance ?: return
        AlertDialog.Builder(this)
            .setTitle(R.string.dialog_clear_data_title)
            .setMessage(getString(R.string.dialog_clear_data_body, current.appName))
            .setNegativeButton(R.string.cancel, null)
            .setPositiveButton(R.string.action_clear_data) { _, _ -> clearData(current) }
            .show()
    }

    private fun clearData(current: Instance) {
        lifecycleScope.launch {
            val cleared = withContext(Dispatchers.IO) {
                engine.instanceManager.clearInstanceData(current.id).isSuccess
            }
            toast(getString(if (cleared) R.string.message_data_cleared else R.string.message_clear_failed))
            refresh()
        }
    }

    // ── Helpers ───────────────────────────────────────────────────────────────

    private fun toast(message: String) {
        Toast.makeText(this, message, Toast.LENGTH_SHORT).show()
    }

    private fun versionName(): String = runCatching {
        packageManager.getPackageInfo(packageName, 0).versionName.orEmpty()
    }.getOrDefault("")

    private companion object {
        /** The artwork shipped in the project's assets folder. */
        const val WALLPAPER_ASSET = "1795549016113.jpg"

        const val APK_MIME = "application/vnd.android.package-archive"

        const val INTRO_DURATION_MS = 520L
        const val WALLPAPER_FADE_MS = 420L
        const val GLOW_PERIOD_MS = 1900L
        const val GLOW_MIN = 0.25f
        const val GLOW_MAX = 0.55f
        const val SHINE_SWEEP_MS = 1150L
        const val SHINE_PERIOD_MS = 3400L
        const val SHINE_RETRY_MS = 300L

        const val MENU_REFRESH = 1
        const val MENU_CLEAR = 2
    }
}
