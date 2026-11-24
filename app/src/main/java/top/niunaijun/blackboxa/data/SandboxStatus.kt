package top.niunaijun.blackboxa.data

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
    /** Nothing to import automatically: the user has to pick an APK file. */
    val needsApk: Boolean = false,
    /** The game was cloned into the sandbox during this provisioning pass. */
    val importedNow: Boolean = false,
    /** Sandboxed game native lib dir (for the Python SDK dumper). */
    val nativeLibDir: String? = null,
    /** Sandboxed game data dir (for the Python SDK dumper). */
    val dataDir: String? = null,
    /** Sandboxed game source dir (for the Python SDK dumper). */
    val sourceDir: String? = null
)

/** Result of a single import/launch/uninstall operation. */
data class OperationResult(
    val success: Boolean,
    val message: String? = null
)
