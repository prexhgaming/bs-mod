package top.niunaijun.blackboxa.data

import android.content.pm.ApplicationInfo
import android.graphics.drawable.Drawable
import android.net.Uri
import android.util.Log
import top.niunaijun.blackbox.BlackBoxCore
import top.niunaijun.blackboxa.GameTarget
import top.niunaijun.blackboxa.app.App
import top.niunaijun.blackboxa.bean.AppInfo


/**
 * Everything that touches the BlackBox engine lives here.
 *
 * The whole point of this build is that the user never has to press a "+"
 * button: if the game is installed on the phone, [provision] clones it into the
 * sandbox automatically, and Google Play services are cloned first so the game
 * has a working GMS environment.
 */
class GameRepository {

    /** Coarse progress of [provision], mapped to strings by the ViewModel. */
    enum class Stage { WAITING, GMS, GAME }

    /**
     * Makes the sandbox usable: waits for the engine, clones GMS, clones the game.
     *
     * Safe to call repeatedly: every step is skipped when already done.
     */
    fun provision(onStage: (Stage) -> Unit = {}): SandboxStatus {
        onStage(Stage.WAITING)
        val servicesReady = awaitServices()

        var gmsSupported = isGmsSupported()
        var gmsInstalled = isGmsInstalled()
        var gameOnDevice = isGameOnDevice()
        var gameInSandbox = isGameInSandbox()
        var message: String? = null
        var importedNow = false

        if (!servicesReady) {
            Log.w(TAG, "Sandbox service did not become ready in time")
            return SandboxStatus(
                gmsSupported = gmsSupported,
                gmsInstalled = gmsInstalled,
                gameOnDevice = gameOnDevice,
                gameInSandbox = gameInSandbox,
                message = "The sandbox service did not start. Restart the app and try again.",
                needsApk = !gameOnDevice && !gameInSandbox
            )
        }

        // 1. Clone the game first — it must be present in the sandbox before GMS.
        if (gameOnDevice && !gameInSandbox) {
            onStage(Stage.GAME)
            val result = runCatching {
                BlackBoxCore.get().installPackageAsUser(GameTarget.PACKAGE_NAME, GameTarget.USER_ID)
            }.getOrNull()
            gameInSandbox = isGameInSandbox()
            if (result == null || !result.success) {
                message = result?.msg?.takeIf { it.isNotBlank() } ?: "The game could not be imported."
                Log.w(TAG, "Game import failed: ${result?.msg}")
            } else {
                importedNow = true
                Log.i(TAG, "Game imported into user ${GameTarget.USER_ID}")
            }
        }

        // 2. GMS after the game is installed, so Play services are ready for it.
        if (gmsSupported && !gmsInstalled) {
            onStage(Stage.GMS)
            val result = runCatching {
                BlackBoxCore.get().installGms(GameTarget.USER_ID)
            }.getOrNull()
            gmsInstalled = isGmsInstalled()
            if (result == null || !result.success) {
                Log.w(TAG, "GMS import failed: ${result?.msg}")
            } else {
                Log.i(TAG, "GMS imported into user ${GameTarget.USER_ID}")
            }
        }

        // Refresh the host-side flags in case something changed while we worked.
        gameOnDevice = isGameOnDevice()
        gmsSupported = isGmsSupported()

        return SandboxStatus(
            gmsSupported = gmsSupported,
            gmsInstalled = gmsInstalled,
            gameOnDevice = gameOnDevice,
            gameInSandbox = gameInSandbox,
            message = message,
            needsApk = !gameOnDevice && !gameInSandbox,
            importedNow = importedNow
        )
    }

    /** Apps currently installed inside the sandbox, target game first. */
    fun listSandboxApps(): List<AppInfo> {
        val installed = runCatching {
            BlackBoxCore.get().getInstalledApplications(0, GameTarget.USER_ID)
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
        val result = runCatching {
            BlackBoxCore.get().installPackageAsUser(uri, GameTarget.USER_ID)
        }.getOrNull()
        return if (result == null) {
            OperationResult(false, "The APK could not be read.")
        } else {
            OperationResult(result.success, result.msg)
        }
    }

    fun launch(packageName: String): Boolean {
        return runCatching {
            // Nothing to hand over: the in-game menu is mapped by the guest
            // lifecycle hook inside the game's own process (ZynBs.load()).
            BlackBoxCore.get().launchApk(packageName, GameTarget.USER_ID)
        }.getOrDefault(false)
    }

    fun uninstall(packageName: String): OperationResult {
        return runCatching {
            BlackBoxCore.get().uninstallPackageAsUser(packageName, GameTarget.USER_ID)
            OperationResult(true)
        }.getOrElse { OperationResult(false, it.message) }
    }

    fun clearData(packageName: String): OperationResult {
        return runCatching {
            BlackBoxCore.get().clearPackage(packageName, GameTarget.USER_ID)
            OperationResult(true)
        }.getOrElse { OperationResult(false, it.message) }
    }

    fun isGameInSandbox(): Boolean = runCatching {
        BlackBoxCore.get().isInstalled(GameTarget.PACKAGE_NAME, GameTarget.USER_ID)
    }.getOrDefault(false)

    fun isGameOnDevice(): Boolean = runCatching {
        App.getContext().packageManager.getPackageInfo(GameTarget.PACKAGE_NAME, 0)
    }.isSuccess

    private fun isGmsSupported(): Boolean = runCatching {
        BlackBoxCore.get().isSupportGms()
    }.getOrDefault(false)

    private fun isGmsInstalled(): Boolean = runCatching {
        BlackBoxCore.get().isInstallGms(GameTarget.USER_ID)
    }.getOrDefault(false)

    /**
     * The engine binds its package service asynchronously. Every engine call is
     * useless (and can throw) until it answers, so poll it once per launch.
     */
    private fun awaitServices(): Boolean {
        repeat(SERVICE_READY_RETRIES) { attempt ->
            val installed = runCatching {
                BlackBoxCore.get().getInstalledApplications(0, GameTarget.USER_ID)
            }.getOrNull()
            if (installed != null) {
                return true
            }
            if (attempt < SERVICE_READY_RETRIES - 1) {
                runCatching { Thread.sleep(SERVICE_READY_DELAY_MS) }
            }
        }
        return false
    }

    private fun toAppInfo(info: ApplicationInfo): AppInfo? {
        val packageName = info.packageName ?: return null
        if (packageName.isBlank()) return null

        val label = runCatching {
            BlackBoxCore.getPackageManager().getApplicationLabel(info).toString()
        }.getOrElse { packageName }

        val icon = runCatching {
            BlackBoxCore.getPackageManager().getApplicationIcon(info)
        }.getOrNull()

        return AppInfo(
            name = label,
            icon = icon,
            packageName = packageName,
            sourceDir = info.sourceDir ?: "",
            isXpModule = false
        )
    }

    /**
     * Icon of the target game: the copy inside the sandbox is preferred because
     * that is what the player actually launches, the host install is the
     * fallback so the launcher still looks right before provisioning finishes.
     */
    fun gameIcon(): Drawable? = runCatching {
        val sandboxInfo = sandboxGameInfo()
        if (sandboxInfo != null) {
            runCatching {
                BlackBoxCore.getPackageManager().getApplicationIcon(sandboxInfo)
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
                BlackBoxCore.getPackageManager().getApplicationLabel(sandboxInfo).toString()
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
        BlackBoxCore.get().getInstalledApplications(0, GameTarget.USER_ID)
            .firstOrNull { it.packageName == GameTarget.PACKAGE_NAME }
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
