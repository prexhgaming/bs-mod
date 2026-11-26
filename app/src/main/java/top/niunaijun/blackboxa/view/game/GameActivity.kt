package top.niunaijun.blackboxa.view.game

import android.animation.Animator
import android.animation.AnimatorListenerAdapter
import android.animation.ObjectAnimator
import android.animation.ValueAnimator
import android.content.Intent
import android.content.res.ColorStateList
import android.graphics.Bitmap
import android.graphics.BitmapFactory
import android.net.Uri
import android.os.Bundle
import android.view.MotionEvent
import android.view.View
import android.view.animation.AccelerateDecelerateInterpolator
import android.view.animation.DecelerateInterpolator
import android.widget.PopupMenu
import androidx.activity.result.contract.ActivityResultContracts
import androidx.appcompat.app.AppCompatActivity
import androidx.core.content.ContextCompat
import androidx.core.view.ViewCompat
import androidx.core.view.WindowCompat
import androidx.core.view.WindowInsetsCompat
import androidx.core.view.isVisible
import androidx.core.view.updatePadding
import androidx.lifecycle.ViewModelProvider
import androidx.lifecycle.lifecycleScope
import com.afollestad.materialdialogs.MaterialDialog
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch
import kotlinx.coroutines.withContext
import top.niunaijun.blackboxa.GameTarget
import top.niunaijun.blackboxa.R
import top.niunaijun.blackboxa.data.GameRepository
import top.niunaijun.blackboxa.data.SandboxStatus
import top.niunaijun.blackboxa.databinding.ActivityGameBinding
import top.niunaijun.blackboxa.util.InjectionUtil
import top.niunaijun.blackboxa.util.inflate
import top.niunaijun.blackboxa.util.toast
import kotlin.math.abs

/**
 * Dedicated launcher for [GameTarget.PACKAGE_NAME].
 *
 * The screen is intentionally about a single game: artwork first, one identity,
 * one call to action. The sandbox engine still clones the game and provisions
 * Google Play services behind the scenes — that work is invisible here and is
 * only ever described with neutral, generic wording.
 */
class GameActivity : AppCompatActivity() {

    private val binding: ActivityGameBinding by inflate()

    private lateinit var viewModel: GameViewModel

    /** Label currently shown for the game (live value, used in copy). */
    private var gameLabel: String = GameTarget.displayName()

    /** True when the notice action should open the APK picker instead of retrying. */
    private var noticePicksApk = false

    private var glowPulse: ValueAnimator? = null
    private var shineAnim: ValueAnimator? = null
    private var introPlayed = false

    /** Alpha the CTA should settle at, so the intro does not undo the state. */
    private var ctaAlpha = 1f

    private val pickApk =
        registerForActivityResult(ActivityResultContracts.OpenDocument()) { uri: Uri? ->
            if (uri != null) {
                runCatching {
                    contentResolver.takePersistableUriPermission(
                        uri,
                        Intent.FLAG_GRANT_READ_URI_PERMISSION
                    )
                }
                viewModel.importApk(uri)
            }
        }

    /** Loops the highlight sweep across the CTA while the screen is alive. */
    private val shineTick = object : Runnable {
        override fun run() {
            val button = binding.launchButton
            val shine = binding.buttonShine
            if (button.width <= 0 || isFinishing || isDestroyed) {
                button.postDelayed(this, SHINE_RETRY_MS)
                return
            }

            val start = -shine.width.toFloat()
            val end = button.width + shine.width.toFloat()
            val sweep = ValueAnimator.ofFloat(0f, 1f).apply {
                duration = SHINE_SWEEP_MS
                interpolator = AccelerateDecelerateInterpolator()
                addUpdateListener { animator ->
                    val fraction = animator.animatedValue as Float
                    shine.translationX = start + (end - start) * fraction
                    // Triangle envelope: fade in, peak mid-sweep, fade out.
                    shine.alpha = 0.65f * (1f - abs(2f * fraction - 1f))
                }
                addListener(object : AnimatorListenerAdapter() {
                    override fun onAnimationEnd(animation: Animator) {
                        shine.alpha = 0f
                    }
                })
            }
            shineAnim?.cancel()
            shineAnim = sweep
            sweep.start()
            button.postDelayed(this, SHINE_PERIOD_MS)
        }
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(binding.root)

        viewModel = ViewModelProvider(this, InjectionUtil.getGameFactory())
            .get(GameViewModel::class.java)

        goEdgeToEdge()
        bindBrandCopy()
        setupActions()
        observe()

        loadHeroArtwork()

        if (savedInstanceState == null) {
            viewModel.provision()
        }
    }

    override fun onResume() {
        super.onResume()
        val status = viewModel.status.value
        // Coming back after installing the game? Import it right away.
        // Only retry when the game is actually on the host device, otherwise we
        // would spin in an endless provision loop.
        if (status != null && !status.busy && !status.needsApk && !status.gameInSandbox &&
            status.gameOnDevice
        ) {
            viewModel.provision(force = true)
        } else {
            viewModel.refresh()
        }
    }

    override fun onDestroy() {
        binding.launchButton.removeCallbacks(shineTick)
        shineAnim?.cancel()
        glowPulse?.cancel()
        super.onDestroy()
    }

    // ------------------------------------------------------------------ layout

    private fun goEdgeToEdge() {
        WindowCompat.setDecorFitsSystemWindows(window, false)
        val horizontal = resources.getDimensionPixelSize(R.dimen.launcher_gutter)
        ViewCompat.setOnApplyWindowInsetsListener(binding.content) { view, insets ->
            val bars = insets.getInsets(WindowInsetsCompat.Type.systemBars())
            view.updatePadding(
                left = horizontal,
                right = horizontal,
                top = bars.top + resources.getDimensionPixelSize(R.dimen.launcher_top_gap),
                bottom = bars.bottom + resources.getDimensionPixelSize(R.dimen.launcher_bottom_gap)
            )
            insets
        }
    }

    private fun bindBrandCopy() {
        binding.footer.text = getString(R.string.launcher_footer, versionName())
    }

    private fun setupActions() {
        binding.launchButton.setOnClickListener { onPrimaryAction() }

        // Subtle press feedback: the CTA shrinks a hair under the finger.
        binding.launchButton.setOnTouchListener { view, event ->
            when (event.action) {
                MotionEvent.ACTION_DOWN -> view.animate()
                    .scaleX(0.97f).scaleY(0.97f).setDuration(110).start()

                MotionEvent.ACTION_UP, MotionEvent.ACTION_CANCEL -> view.animate()
                    .scaleX(1f).scaleY(1f).setDuration(150).start()
            }
            false
        }

        binding.moreButton.setOnClickListener { showOverflow() }

        binding.noticeAction.setOnClickListener {
            if (noticePicksApk) {
                pickApk.launch(arrayOf("*/*"))
            } else {
                viewModel.provision(force = true)
            }
        }
    }

    private fun showOverflow() {
        val popup = PopupMenu(this, binding.moreButton)
        popup.menuInflater.inflate(R.menu.menu_launcher, popup.menu)
        popup.setOnMenuItemClickListener { item ->
            when (item.itemId) {
                R.id.action_refresh -> {
                    viewModel.provision(force = true)
                    true
                }

                R.id.action_clear_data -> {
                    confirmClearData()
                    true
                }

                R.id.action_select_apk -> {
                    pickApk.launch(arrayOf("*/*"))
                    true
                }

                else -> false
            }
        }
        popup.show()
    }

    private fun confirmClearData() {
        MaterialDialog(this).show {
            title(text = getString(R.string.dialog_clear_data_title))
            message(text = getString(R.string.dialog_clear_data_body, gameLabel))
            positiveButton(R.string.action_clear_data) {
                viewModel.clearAppData(GameTarget.PACKAGE_NAME)
            }
            negativeButton(R.string.cancel) { }
        }
    }

    private fun onPrimaryAction() {
        val status = viewModel.status.value ?: SandboxStatus()
        when {
            status.busy -> Unit
            status.gameInSandbox -> viewModel.launchApp(GameTarget.PACKAGE_NAME)
            status.gameOnDevice -> viewModel.provision(force = true)
            else -> pickApk.launch(arrayOf("*/*"))
        }
    }

    // -------------------------------------------------------------- wallpaper

    private fun loadHeroArtwork() {
        lifecycleScope.launch {
            val bitmap = withContext(Dispatchers.IO) { decodeWallpaper() }
            if (isFinishing || isDestroyed) return@launch
            if (bitmap != null) {
                binding.heroImage.setImageBitmap(bitmap)
            }
            playIntro()
            startAmbientMotion()
        }
    }

    /**
     * Decodes the artwork straight from assets, downsampled to the screen width
     * and in RGB_565 so a full-bleed background stays cheap on older devices.
     */
    private fun decodeWallpaper(): Bitmap? = runCatching {
        val targetWidth = resources.displayMetrics.widthPixels.coerceAtLeast(1)

        val bounds = BitmapFactory.Options().apply { inJustDecodeBounds = true }
        assets.open(WALLPAPER_ASSET).use { BitmapFactory.decodeStream(it, null, bounds) }

        var sample = 1
        while (bounds.outWidth / (sample * 2) >= targetWidth) {
            sample *= 2
        }

        val options = BitmapFactory.Options().apply {
            inSampleSize = sample
            inPreferredConfig = Bitmap.Config.RGB_565
        }
        assets.open(WALLPAPER_ASSET).use { BitmapFactory.decodeStream(it, null, options) }
    }.getOrNull()

    // ------------------------------------------------------------- animation

    /** Staggered entrance: brand, hero, identity, status, then the CTA. */
    private fun playIntro() {
        if (introPlayed) return
        introPlayed = true

        val rise = resources.getDimensionPixelSize(R.dimen.launcher_rise).toFloat()
        val sequence = listOf(
            binding.wordmark to 0L,
            binding.brandSub to 40L,
            binding.moreButton to 80L,
            binding.iconGlow to 120L,
            binding.gameIcon to 150L,
            binding.gameTitle to 210L,
            binding.gameTagline to 260L,
            binding.statusPill to 310L,
            binding.launchHint to 360L,
            binding.ctaGlow to 400L,
            binding.footer to 500L
        )

        sequence.forEach { (view, delay) ->
            view.alpha = 0f
            view.translationY = rise
            view.animate()
                .alpha(1f)
                .translationY(0f)
                .setStartDelay(delay)
                .setDuration(INTRO_DURATION_MS)
                .setInterpolator(DecelerateInterpolator())
                .start()
        }

        // The CTA lands on the alpha its current state asks for.
        binding.launchButton.alpha = 0f
        binding.launchButton.translationY = rise
        binding.launchButton.animate()
            .alpha(ctaAlpha)
            .translationY(0f)
            .setStartDelay(430L)
            .setDuration(INTRO_DURATION_MS)
            .setInterpolator(DecelerateInterpolator())
            .start()

        // The icon settles with a little more presence than the rest.
        binding.gameIcon.scaleX = 0.82f
        binding.gameIcon.scaleY = 0.82f
        binding.gameIcon.animate()
            .scaleX(1f)
            .scaleY(1f)
            .setStartDelay(150L)
            .setDuration(620L)
            .setInterpolator(DecelerateInterpolator())
            .start()
    }

    /** Slow breathing halos + the repeating light sweep on the CTA. */
    private fun startAmbientMotion() {
        glowPulse?.cancel()
        val pulse = ValueAnimator.ofFloat(0.18f, 0.5f).apply {
            duration = GLOW_PERIOD_MS
            repeatCount = ValueAnimator.INFINITE
            repeatMode = ValueAnimator.REVERSE
            addUpdateListener { animator ->
                val value = animator.animatedValue as Float
                binding.ctaGlow.alpha = value
                binding.iconGlow.alpha = value * 0.9f
            }
        }
        glowPulse = pulse

        // Let the entrance finish before the halos start breathing.
        binding.ctaGlow.postDelayed({
            if (!isFinishing && !isDestroyed) pulse.start()
        }, INTRO_DURATION_MS + 140L)

        binding.launchButton.removeCallbacks(shineTick)
        binding.launchButton.postDelayed(shineTick, INTRO_DURATION_MS + SHINE_PERIOD_MS / 2)
    }

    /** Short horizontal shake used when a launch fails. */
    private fun shakeCta() {
        ObjectAnimator.ofFloat(
            binding.launchButton,
            View.TRANSLATION_X,
            0f, -16f, 16f, -10f, 10f, 0f
        ).apply {
            duration = 420L
            start()
        }
    }

    // --------------------------------------------------------------- observe

    private fun observe() {
        viewModel.status.observe(this) { status ->
            render(status)
        }

        viewModel.stage.observe(this) { stage ->
            if (viewModel.status.value?.busy == true) {
                binding.launchHint.text = stageLabel(stage)
            }
        }

        viewModel.gameLabel.observe(this) { label ->
            gameLabel = label
            binding.gameTitle.text = label.uppercase()
            binding.gameIcon.contentDescription =
                getString(R.string.launcher_icon_desc, label)
        }

        viewModel.gameIcon.observe(this) { icon ->
            if (icon != null) {
                binding.gameIcon.setImageDrawable(icon)
            }
        }

        viewModel.message.observe(this) { message ->
            if (!message.isNullOrBlank()) {
                toast(message)
                viewModel.consumeMessage()
            }
        }

        viewModel.launchFailed.observe(this) { failed ->
            if (failed) {
                toast(R.string.toast_launch_failed)
                shakeCta()
                viewModel.consumeLaunchFailed()
            }
        }
    }

    // ---------------------------------------------------------------- render

    private fun render(status: SandboxStatus) {
        binding.preparingBar.isVisible = status.busy
        renderNotice(status)

        when {
            status.busy -> {
                pill(
                    R.string.launcher_status_preparing,
                    R.color.launcher_pill_neutral_bg,
                    R.color.launcher_pill_neutral_text
                )
                binding.launchLabel.setText(R.string.action_preparing)
                binding.launchIcon.isVisible = false
                binding.launchHint.text =
                    stageLabel(viewModel.stage.value ?: GameRepository.Stage.WAITING)
                dial(1f)
            }

            status.gameInSandbox -> {
                pill(
                    R.string.launcher_status_ready,
                    R.color.launcher_pill_ready_bg,
                    R.color.launcher_pill_ready_text
                )
                binding.launchLabel.setText(R.string.action_launch)
                binding.launchIcon.isVisible = true
                binding.launchHint.setText(R.string.launcher_hint_ready)
                dial(1f)
            }

            else -> {
                pill(
                    R.string.launcher_status_missing,
                    R.color.launcher_pill_warn_bg,
                    R.color.launcher_pill_warn_text
                )
                if (status.needsApk) {
                    binding.launchLabel.setText(R.string.action_install)
                    binding.launchHint.text =
                        getString(R.string.launcher_hint_missing, gameLabel)
                } else {
                    binding.launchLabel.setText(R.string.action_launch)
                    binding.launchHint.setText(R.string.launcher_hint_preparing)
                }
                binding.launchIcon.isVisible = true
                dial(0.78f)
            }
        }
    }

    private fun renderNotice(status: SandboxStatus) {
        val missing = !status.busy && status.needsApk
        // A failed Google clone lands here too: the game imported fine, so
        // nothing else would report it and the failure used to be Logcat-only.
        val detail = status.message ?: status.gmsError
        val failed = !status.busy && !status.needsApk && !detail.isNullOrBlank()
        val visible = missing || failed

        binding.noticeCard.isVisible = visible
        if (!visible) return

        if (missing) {
            noticePicksApk = true
            binding.noticeTitle.text = getString(R.string.notice_missing_title, gameLabel)
            binding.noticeBody.setText(R.string.notice_missing_body)
            binding.noticeAction.setText(R.string.action_select_apk)
        } else {
            noticePicksApk = false
            binding.noticeTitle.setText(R.string.notice_error_title)
            binding.noticeBody.text = detail
            binding.noticeAction.setText(R.string.action_retry)
        }
    }

    private fun pill(textRes: Int, backgroundColor: Int, textColor: Int) {
        binding.statusPill.setText(textRes)
        binding.statusPill.backgroundTintList =
            ColorStateList.valueOf(ContextCompat.getColor(this, backgroundColor))
        binding.statusPill.setTextColor(ContextCompat.getColor(this, textColor))
    }

    private fun dial(alpha: Float) {
        ctaAlpha = alpha
        binding.launchButton.alpha = alpha
    }

    /** Generic wording only — the engine's internals never surface on screen. */
    private fun stageLabel(stage: GameRepository.Stage): String = getString(
        when (stage) {
            GameRepository.Stage.WAITING -> R.string.launcher_stage_waiting
            GameRepository.Stage.GMS -> R.string.launcher_stage_environment
            GameRepository.Stage.GAME -> R.string.launcher_stage_game
        }
    )

    private fun versionName(): String = runCatching {
        packageManager.getPackageInfo(packageName, 0).versionName.orEmpty()
    }.getOrDefault("")

    companion object {
        private const val TAG = "GameActivity"

        /** The artwork shipped in the project's assets folder. */
        private const val WALLPAPER_ASSET = "1795549016113.jpg"

        private const val INTRO_DURATION_MS = 520L
        private const val GLOW_PERIOD_MS = 1900L
        private const val SHINE_SWEEP_MS = 1150L
        private const val SHINE_PERIOD_MS = 3400L
        private const val SHINE_RETRY_MS = 300L
    }
}
