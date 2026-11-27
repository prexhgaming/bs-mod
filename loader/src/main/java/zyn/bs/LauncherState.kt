package zyn.bs

/**
 * What the launcher is currently telling the user.
 *
 * Deliberately small: the screen has one action and one status pill, so the
 * whole surface is these four states. [WAITING] and [PREPARING] differ only in
 * wording (engine boot vs. instance creation), never in affordance.
 */
enum class LauncherState {
    /** Booting: reading the package manager, before the instance is known. */
    WAITING,

    /** A container instance is being created or the APK is being imported. */
    PREPARING,

    /** An instance exists and the game can be launched. */
    READY,

    /** The game is not installed and no APK has been imported yet. */
    MISSING
}
