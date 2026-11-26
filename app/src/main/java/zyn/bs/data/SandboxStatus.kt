package zyn.bs.data

/**
 * Snapshot of everything the single screen needs to render.
 *
 * Produced by [GameRepository.provision] and re-rendered after every action so
 * the UI never has to guess the state of the sandbox.
 */
data class SandboxStatus(
    /** Google Play services exist on the host device, so they can be cloned. */
    val gmsSupported: Boolean = false,
    /** Google Play services are already installed inside the sandbox user. */
    val gmsInstalled: Boolean = false,
    /** The target game is installed on the host device. */
    val gameOnDevice: Boolean = false,
    /** The target game is installed inside the sandbox user. */
    val gameInSandbox: Boolean = false,
    /** A blocking operation is running; show the overlay. */
    val busy: Boolean = false,
    /** Human readable failure, rendered in the banner. */
    val message: String? = null,
    /**
     * The Google clone failed while the game itself is fine. Kept separate from
     * [message] so a game import error is never masked, and surfaced in the same
     * banner (the user asked for GMS failures not to be silent).
     */
    val gmsError: String? = null,
    /** Nothing to import automatically: the user has to pick an APK file. */
    val needsApk: Boolean = false,
    /** The game was cloned into the sandbox during this provisioning pass. */
    val importedNow: Boolean = false
)

/** Result of a single import/launch/uninstall operation. */
data class OperationResult(
    val success: Boolean,
    val message: String? = null
)
