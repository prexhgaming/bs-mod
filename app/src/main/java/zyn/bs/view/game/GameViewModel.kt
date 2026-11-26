package zyn.bs.view.game

import android.graphics.drawable.Drawable
import android.net.Uri
import androidx.lifecycle.LiveData
import androidx.lifecycle.MutableLiveData
import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewModelScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch
import kotlinx.coroutines.withContext
import zyn.bs.GameTarget
import zyn.bs.R
import zyn.bs.bean.AppInfo
import zyn.bs.data.GameRepository
import zyn.bs.data.SandboxStatus
import zyn.bs.util.getString

/**
 * Drives the single screen: provisioning, the sandbox app list and per-app actions.
 */
class GameViewModel(private val repo: GameRepository) : ViewModel() {

    private val _status = MutableLiveData(SandboxStatus(busy = true))
    val status: LiveData<SandboxStatus> = _status

    private val _apps = MutableLiveData<List<AppInfo>>(emptyList())
    val apps: LiveData<List<AppInfo>> = _apps

    /** Icon + label of the one game this launcher exists for. */
    private val _gameIcon = MutableLiveData<Drawable?>()
    val gameIcon: LiveData<Drawable?> = _gameIcon

    private val _gameLabel = MutableLiveData(GameTarget.displayName())
    val gameLabel: LiveData<String> = _gameLabel

    /** Provisioning step currently running, used for the progress label. */
    private val _stage = MutableLiveData(GameRepository.Stage.WAITING)
    val stage: LiveData<GameRepository.Stage> = _stage

    private val _message = MutableLiveData<String?>()
    val message: LiveData<String?> = _message

    private val _launchFailed = MutableLiveData<Boolean>()
    val launchFailed: LiveData<Boolean> = _launchFailed

    private var provisioning = false
    private var provisioned = false

    /** Runs the one-time setup (GMS + game import). Safe to call again. */
    fun provision(force: Boolean = false) {
        if (provisioning) return
        if (provisioned && !force) {
            refresh()
            return
        }

        provisioning = true
        _stage.value = GameRepository.Stage.WAITING
        _status.value = (_status.value ?: SandboxStatus()).copy(
            busy = true,
            message = null,
            needsApk = false
        )

        viewModelScope.launch {
            val result = withContext(Dispatchers.IO) {
                repo.provision { stage -> _stage.postValue(stage) }
            }
            provisioned = true
            provisioning = false
            _status.value = result.copy(busy = false)
            loadApps()
            loadIdentity()

            if (result.importedNow) {
                _message.value = getString(
                    R.string.message_game_ready,
                    _gameLabel.value ?: GameTarget.PACKAGE_NAME
                )
            }
        }
    }

    /** Reloads only the game identity + sandbox app list. */
    fun refresh() {
        viewModelScope.launch {
            loadApps()
            loadIdentity()
        }
    }

    private suspend fun loadApps() {
        val list = withContext(Dispatchers.IO) { repo.listSandboxApps() }
        _apps.postValue(list)
    }

    /** Refreshes the hero artwork/label once the sandbox knows about the game. */
    private suspend fun loadIdentity() {
        val icon = withContext(Dispatchers.IO) { repo.gameIcon() }
        val label = withContext(Dispatchers.IO) { repo.gameLabel() }
        _gameIcon.postValue(icon)
        _gameLabel.postValue(label)
    }

    fun launchApp(packageName: String) {
        viewModelScope.launch {
            _status.value = (_status.value ?: SandboxStatus()).copy(busy = true)
            val launched = withContext(Dispatchers.IO) { repo.launch(packageName) }
            _status.value = (_status.value ?: SandboxStatus()).copy(busy = false)
            if (!launched) {
                _launchFailed.value = true
            }
        }
    }

    fun uninstallApp(packageName: String) {
        viewModelScope.launch {
            _status.value = (_status.value ?: SandboxStatus()).copy(busy = true)
            val result = withContext(Dispatchers.IO) { repo.uninstall(packageName) }
            _status.value = (_status.value ?: SandboxStatus()).copy(busy = false)
            _message.value = if (result.success) {
                getString(R.string.message_uninstalled)
            } else {
                result.message ?: getString(R.string.message_uninstall_failed)
            }
            loadApps()
        }
    }

    fun clearAppData(packageName: String) {
        viewModelScope.launch {
            _status.value = (_status.value ?: SandboxStatus()).copy(busy = true)
            val result = withContext(Dispatchers.IO) { repo.clearData(packageName) }
            _status.value = (_status.value ?: SandboxStatus()).copy(busy = false)
            _message.value = if (result.success) {
                getString(R.string.message_data_cleared)
            } else {
                result.message ?: getString(R.string.message_clear_failed)
            }
        }
    }

    /** Fallback import for a device that does not have the game installed. */
    fun importApk(uri: Uri) {
        viewModelScope.launch {
            _status.value = (_status.value ?: SandboxStatus()).copy(busy = true, message = null)
            val result = withContext(Dispatchers.IO) { repo.importFromUri(uri) }
            val refreshed = withContext(Dispatchers.IO) { repo.provision() }
            _status.value = refreshed.copy(busy = false)
            loadApps()
            loadIdentity()

            _message.value = if (result.success) {
                getString(R.string.message_game_imported)
            } else {
                result.message ?: getString(R.string.message_import_failed)
            }
        }
    }

    fun consumeMessage() {
        _message.value = null
    }

    fun consumeLaunchFailed() {
        _launchFailed.value = false
    }
}
