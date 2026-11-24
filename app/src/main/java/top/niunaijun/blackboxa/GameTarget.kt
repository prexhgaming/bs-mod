package top.niunaijun.blackboxa

import android.content.Context
import android.content.pm.PackageManager
import top.niunaijun.blackboxa.app.App

/**
 * Single target this build is dedicated to.
 *
 * The sandbox only exists to run one game, so every flow in the app is
 * hard-wired to [PACKAGE_NAME] and to the main sandbox user. The game's *name*
 * is never hard-coded — it is read from the package manager.
 */
object GameTarget {

    /** Target game package (Knives Out, NetEase). */
    const val PACKAGE_NAME: String = "com.netease.newspike"

    /** BlackBox user the game is installed into. 0 is the main user. */
    const val USER_ID: Int = 0

    /**
     * The target's label, read from this (host) package manager.
     *
     * This is only one of two sources: `GameRepository.gameLabel()` checks the
     * sandbox first, because a cloned game is not visible to the host package
     * manager. Returns [PACKAGE_NAME] while the game is not installed, so no
     * title ever has to be invented.
     */
    fun displayName(context: Context = App.getContext()): String {
        val pm: PackageManager = context.packageManager
        val label = runCatching {
            pm.getApplicationLabel(pm.getApplicationInfo(PACKAGE_NAME, 0)).toString()
        }.getOrNull()
        return label?.takeIf { it.isNotBlank() } ?: PACKAGE_NAME
    }
}
