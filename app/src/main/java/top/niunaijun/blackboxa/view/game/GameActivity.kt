package top.niunaijun.blackboxa.view.game

import android.content.Intent
import android.net.Uri
import android.os.Bundle
import androidx.activity.result.contract.ActivityResultContracts
import androidx.appcompat.app.AppCompatActivity
import androidx.core.content.ContextCompat
import androidx.core.view.isVisible
import androidx.lifecycle.ViewModelProvider
import androidx.recyclerview.widget.GridLayoutManager
import com.afollestad.materialdialogs.MaterialDialog
import top.niunaijun.blackboxa.GameTarget
import top.niunaijun.blackboxa.R
import top.niunaijun.blackboxa.bean.AppInfo
import top.niunaijun.blackboxa.data.GameRepository
import top.niunaijun.blackboxa.data.SandboxStatus
import top.niunaijun.blackboxa.databinding.ActivityGameBinding
import top.niunaijun.blackboxa.util.InjectionUtil
import top.niunaijun.blackboxa.util.inflate
import top.niunaijun.blackboxa.util.toast

/**
 * The only screen of the app.
 *
 * On first launch it provisions the sandbox by itself: Google Play services are
 * cloned first, then the target game is cloned straight from the copy that is
 * installed on the phone. The screen then stays a plain list of what is inside
 * the sandbox, so there is no import button and no extra navigation.
 */
class GameActivity : AppCompatActivity() {

    private val binding: ActivityGameBinding by inflate()

    private lateinit var viewModel: GameViewModel
    private lateinit var adapter: GameAppAdapter

    /** True when the banner action should open the APK picker instead of retrying. */
    private var bannerPicksApk = false

    private val pickApk = registerForActivityResult(ActivityResultContracts.OpenDocument()) { uri: Uri? ->
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

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(binding.root)
        setSupportActionBar(binding.toolbar)
        binding.toolbar.subtitle = getString(R.string.toolbar_subtitle, GameTarget.USER_ID)

        viewModel = ViewModelProvider(this, InjectionUtil.getGameFactory())
            .get(GameViewModel::class.java)

        setupList()
        setupBanner()
        setupMenu()
        observe()

        if (savedInstanceState == null) {
            viewModel.provision()
        }
    }

    override fun onResume() {
        super.onResume()
        val status = viewModel.status.value
        // Coming back after installing the game? Import it right away.
        // Only retry when the game is actually on the host device; otherwise we
        // would spin in an endless provision loop.
        if (status != null && !status.busy && !status.needsApk && !status.gameInSandbox
            && status.gameOnDevice) {
            viewModel.provision(force = true)
        } else {
            viewModel.refresh()
        }
    }

    private fun setupList() {
        adapter = GameAppAdapter(
            onClick = { item -> viewModel.launchApp(item.packageName) },
            onLongClick = { item -> showAppActions(item) }
        )
        binding.recyclerView.layoutManager = GridLayoutManager(this, GRID_COLUMNS)
        binding.recyclerView.adapter = adapter
        binding.recyclerView.setHasFixedSize(true)
    }

    private fun setupBanner() {
        binding.bannerAction.setOnClickListener {
            if (bannerPicksApk) {
                pickApk.launch(arrayOf("*/*"))
            } else {
                viewModel.provision(force = true)
            }
        }
    }

    private fun setupMenu() {
        binding.toolbar.setOnMenuItemClickListener { item ->
            when (item.itemId) {
                R.id.action_refresh -> {
                    viewModel.provision(force = true)
                    true
                }

                else -> false
            }
        }
    }

    private fun observe() {
        viewModel.apps.observe(this) { apps ->
            adapter.submitList(apps)
            binding.listCount.text = apps.size.toString()
            renderEmptyState(apps)
        }

        viewModel.status.observe(this) { status ->
            binding.loadingOverlay.isVisible = status.busy
            renderChips(status)
            renderBanner(status)
            renderEmptyState(viewModel.apps.value.orEmpty())
        }

        viewModel.stage.observe(this) { stage ->
            binding.loadingText.setText(
                when (stage) {
                    GameRepository.Stage.GMS -> R.string.loading_gms
                    GameRepository.Stage.GAME -> R.string.loading_game
                    GameRepository.Stage.WAITING -> R.string.loading_waiting
                }
            )
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
                viewModel.consumeLaunchFailed()
            }
        }
    }

    private fun renderChips(status: SandboxStatus) {
        when {
            !status.gmsSupported -> {
                binding.gmsChip.setText(R.string.chip_gms_unavailable)
                styleChip(binding.gmsChip, R.drawable.bg_chip_warn, R.color.chip_warn_text)
            }

            status.gmsInstalled -> {
                binding.gmsChip.setText(R.string.chip_gms_ready)
                styleChip(binding.gmsChip, R.drawable.bg_chip_ok, R.color.chip_ok_text)
            }

            else -> {
                binding.gmsChip.setText(R.string.chip_gms_pending)
                styleChip(binding.gmsChip, R.drawable.bg_chip_neutral, R.color.chip_neutral_text)
            }
        }

        if (status.gameInSandbox) {
            binding.gameChip.setText(R.string.chip_game_ready)
            styleChip(binding.gameChip, R.drawable.bg_chip_ok, R.color.chip_ok_text)
        } else {
            binding.gameChip.setText(R.string.chip_game_missing)
            styleChip(binding.gameChip, R.drawable.bg_chip_neutral, R.color.chip_neutral_text)
        }
    }

    private fun styleChip(chip: android.widget.TextView, background: Int, textColor: Int) {
        chip.setBackgroundResource(background)
        chip.setTextColor(ContextCompat.getColor(this, textColor))
    }

    private fun renderBanner(status: SandboxStatus) {
        val show = status.needsApk || (status.message != null && !status.busy)
        binding.banner.isVisible = show
        if (!show) return

        if (status.needsApk) {
            bannerPicksApk = true
            binding.bannerTitle.text =
                getString(R.string.banner_game_missing_title, GameTarget.DISPLAY_NAME)
            binding.bannerBody.text =
                getString(R.string.banner_game_missing_body, GameTarget.DISPLAY_NAME)
            binding.bannerAction.setText(R.string.action_select_apk)
        } else {
            bannerPicksApk = false
            binding.bannerTitle.setText(R.string.banner_error_title)
            binding.bannerBody.text = status.message
            binding.bannerAction.setText(R.string.action_retry)
        }
    }

    private fun renderEmptyState(apps: List<AppInfo>) {
        val status = viewModel.status.value ?: SandboxStatus()
        val show = apps.isEmpty() && !status.busy && !status.needsApk
        binding.emptyView.isVisible = show
        if (show) {
            binding.emptyTitle.setText(R.string.empty_title)
            binding.emptyHint.text = getString(R.string.empty_hint, GameTarget.DISPLAY_NAME)
        }
    }

    private fun showAppActions(item: AppInfo) {
        MaterialDialog(this).show {
            title(text = item.name)
            message(text = getString(R.string.dialog_app_message, item.packageName))
            positiveButton(R.string.action_clear_data) { viewModel.clearAppData(item.packageName) }
            neutralButton(R.string.action_uninstall) { viewModel.uninstallApp(item.packageName) }
            negativeButton(R.string.cancel) { }
        }
    }

    companion object {
        private const val GRID_COLUMNS = 4
    }
}
