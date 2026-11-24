package top.niunaijun.blackboxa

/**
 * Single target this build is dedicated to.
 *
 * The sandbox only exists to run one game, so every flow in the app is
 * hard-wired to [PACKAGE_NAME] and to the main sandbox user.
 */
object GameTarget {

    /** Knives Out (NetEase). */
    const val PACKAGE_NAME: String = "com.netease.newspike"

    /** Human readable name, used before the real label can be resolved. */
    const val DISPLAY_NAME: String = "Knives Out"

    /** BlackBox user the game is installed into. 0 is the main user. */
    const val USER_ID: Int = 0
}
