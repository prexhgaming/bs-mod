package zyn.bs.data

import android.content.pm.ApplicationInfo
import android.graphics.drawable.Drawable
import android.net.Uri
import android.util.Log
import com.lody.virtual.client.core.VirtualCore
import com.lody.virtual.client.ipc.VActivityManager
import com.lody.virtual.client.ipc.VPackageManager
import com.lody.virtual.helper.utils.FileUtils
import zyn.bs.GameTarget
import zyn.bs.app.App
import zyn.bs.bean.AppInfo

/**
 * Everything that touches the VirtualApp engine lives here.
 *
 * The user never presses a "+": if the game is installed on the phone it is
 * cloned into the sandbox automatically. GMS is intentionally ignored —
 * VirtualApp does not have the Google-services problem BlackBox had, so no
 * clone step, no chip, no status line (per user decision).
 */
class GameRepository {

    /** Coarse progress of [provision], mapped to strings by the ViewModel. */
    enum class Stage { WAITING, GMS, GAME }

    /**
     * Makes the sandbox usable: waits for the engine, clones the game.
     * Safe to call repeatedly: every step is skipped when already done.
     */
    fun provision(onStage: (Stage) -> Unit = {}): SandboxStatus {
        onStage(Stage.WAITING)
        val servicesReady = awaitServices()

        var gameOnDevice = isGameOnDevice()
        var gameInSandbox = isGameInSandbox()
        var message: String? = null
        var importedNow = false

        if (!servicesReady) {
            Log.w(TAG, "Sandbox engine did not become ready in time")
            return SandboxStatus(
                gmsSupported = false,
                gmsInstalled = true,
                gameOnDevice = gameOnDevice,
                gameInSandbox = gameInSandbox,
                message = "The sandbox service did not start. Restart the app and try again.",
                needsApk = !gameOnDevice && !gameInSandbox
            )
        }

        if (gameOnDevice && !gameInSandbox) {
            onStage(Stage.GAME)
            val ok = runCatching {
                VirtualCore.get().installPackageAsUser(GameTarget.USER_ID, GameTarget.PACKAGE_NAME)
            }.getOrDefault(false)
            gameInSandbox = isGameInSandbox()
            if (!ok) {
                message = "The game could not be imported."
                Log.w(TAG, "Game import failed")
            } else {
                importedNow = true
                Log.i(TAG, "Game imported into user ${GameTarget.USER_ID}")
            }
        }

        gameOnDevice = isGameOnDevice()

        return SandboxStatus(
            gmsSupported = false,
            gmsInstalled = true,
            gameOnDevice = gameOnDevice,
            gameInSandbox = gameInSandbox,
            message = message,
            needsApk = !gameOnDevice && !gameInSandbox,
            importedNow = importedNow,
            gmsError = null
        )
    }

    /** Apps currently installed inside the sandbox, target game first. */
    fun listSandboxApps(): List<AppInfo> {
        val installed = runCatching {
            VPackageManager.get().getInstalledApplications(0, GameTarget.USER_ID)
        }.getOrNull() ?: return emptyList()

        return installed
            .mapNotNull { info -> runCatching { toAppInfo(info) }.getOrNull() }
            .sortedWith(
                compareByDescending<AppInfo> { it.packageName == GameTarget.PACKAGE_NAME }
                    .thenBy { it.name.lowercase() }
            )
    }

    /** Fallback path: imports an APK the user picked with the system file picker. */
    fun importFromUri(uri: Uri): OperationResult {
        awaitServices()
        return runCatching {
            val path = FileUtils.getFilePathByUri(App.getContext(), uri)
                ?: return OperationResult(false, "The APK could not be read.")
            val result = VirtualCore.get().installPackage(path, null)
            OperationResult(result.isSuccess, result.error)
        }.getOrElse { OperationResult(false, it.message) }
    }

    fun launch(packageName: String): Boolean = runCatching {
        // Nothing to hand over: the in-game menu is mapped by the guest
        // lifecycle hook inside the game's own process (ZynBs.load()).
        VActivityManager.get().launchApp(GameTarget.USER_ID, packageName)
    }.getOrDefault(false)

    fun uninstall(packageName: String): OperationResult {
        return runCatching {
            val ok = VirtualCore.get().uninstallPackageAsUser(packageName, GameTarget.USER_ID)
            OperationResult(ok)
        }.getOrElse { OperationResult(false, it.message) }
    }

    fun clearData(packageName: String): OperationResult {
        return runCatching {
            val ok = VirtualCore.get().cleanPackageData(packageName, GameTarget.USER_ID)
            OperationResult(ok)
        }.getOrElse { OperationResult(false, it.message) }
    }

    fun isGameInSandbox(): Boolean = runCatching {
        VirtualCore.get().isAppInstalled(GameTarget.PACKAGE_NAME)
    }.getOrDefault(false)

    fun isGameOnDevice(): Boolean = runCatching {
        App.getContext().packageManager.getPackageInfo(GameTarget.PACKAGE_NAME, 0)
    }.isSuccess

    /**
     * The engine binds its package service asynchronously. Poll once per launch.
     */
    private fun awaitServices(): Boolean {
        repeat(SERVICE_READY_RETRIES) {
            if (runCatching { VirtualCore.get().isStartup() }.getOrDefault(false)) {
                return true
            }
            runCatching { Thread.sleep(SERVICE_READY_DELAY_MS) }
        }
        return false
    }

    private fun toAppInfo(info: ApplicationInfo): AppInfo? {
        val packageName = info.packageName ?: return null
        if (packageName.isBlank()) return null

        val label = runCatching {
            VirtualCore.get().getPackageManager().getApplicationLabel(info).toString()
        }.getOrElse { packageName }

        val icon = runCatching {
            VirtualCore.get().getPackageManager().getApplicationIcon(info)
        }.getOrNull()

        return AppInfo(
            name = label,
            icon = icon,
            packageName = packageName,
            sourceDir = info.sourceDir ?: "",
            isXpModule = false
        )
    }

    /** Icon of the target game: sandbox copy preferred, host install fallback. */
    fun gameIcon(): Drawable? = runCatching {
        val sandboxInfo = sandboxGameInfo()
        if (sandboxInfo != null) {
            runCatching {
                VirtualCore.get().getPackageManager().getApplicationIcon(sandboxInfo)
            }.getOrNull()
        } else {
            hostGameInfo()?.let { info ->
                runCatching {
                    App.getContext().packageManager.getApplicationIcon(info)
                }.getOrNull()
            }
        }
    }.getOrNull()

    /** Label of the target game, read from the package manager (sandbox first). */
    fun gameLabel(): String = runCatching {
        val sandboxInfo = sandboxGameInfo()
        if (sandboxInfo != null) {
            runCatching {
                VirtualCore.get().getPackageManager().getApplicationLabel(sandboxInfo).toString()
            }.getOrNull()
        } else {
            hostGameInfo()?.let { info ->
                runCatching {
                    App.getContext().packageManager.getApplicationLabel(info).toString()
                }.getOrNull()
            }
        }
    }.getOrNull()?.takeIf { it.isNotBlank() } ?: GameTarget.displayName()

    private fun sandboxGameInfo(): ApplicationInfo? = runCatching {
        VPackageManager.get().getApplicationInfo(GameTarget.PACKAGE_NAME, 0, GameTarget.USER_ID)
    }.getOrNull()

    private fun hostGameInfo(): ApplicationInfo? = runCatching {
        App.getContext().packageManager.getApplicationInfo(GameTarget.PACKAGE_NAME, 0)
    }.getOrNull()

    companion object {
        private const val TAG = "GameRepository"
        private const val SERVICE_READY_RETRIES = 20
        private const val SERVICE_READY_DELAY_MS = 250L
    }
}
