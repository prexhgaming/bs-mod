package zyn.bs.app

import android.content.Context
import android.content.SharedPreferences
import android.util.Log
import com.lody.virtual.client.core.VirtualCore
import zyn.bs.GameTarget

/**
 * Engine-independent manager for sandbox lifecycle.
 * All BlackBoxCore calls have been replaced with VirtualApp equivalents.
 * GMS is intentionally ignored per user decision — the game handles its own GMS.
 */
object AppManager {

    private const val TAG = "AppManager"

    /** Called once from [App.onCreate] after VirtualCore startup. */
    @JvmStatic
    fun doOnCreate(context: Context) {
        try {
            Log.d(TAG, "doOnCreate: VirtualCore already started; lifecycle hook registered in App")
        } catch (e: Throwable) {
            Log.e(TAG, "doOnCreate failed", e)
        }
    }

    /** Legacy entry point kept for callers; VirtualCore is initialised in [App.onCreate]. */
    @JvmStatic
    fun doAttachBaseContext(context: Context) {
        try {
            Log.d(TAG, "doAttachBaseContext: VirtualCore already initialised")
        } catch (e: Throwable) {
            Log.e(TAG, "doAttachBaseContext failed", e)
        }
    }

    /** Install the target game into the sandbox user. Returns true on success. */
    @JvmStatic
    fun installGame(): Boolean = try {
        VirtualCore.get().installPackageAsUser(GameTarget.USER_ID, GameTarget.PACKAGE_NAME)
    } catch (e: Throwable) {
        Log.e(TAG, "installGame failed", e)
        false
    }

    /** Check whether the target game is installed in the sandbox user. */
    @JvmStatic
    fun isGameInSandbox(): Boolean = try {
        VirtualCore.get().isAppInstalled(GameTarget.PACKAGE_NAME)
    } catch (e: Throwable) {
        Log.e(TAG, "isGameInSandbox failed", e)
        false
    }

    /** Check whether the target game is installed on the host device. */
    @JvmStatic
    fun isGameOnDevice(): Boolean = try {
        App.getContext().packageManager.getPackageInfo(GameTarget.PACKAGE_NAME, 0)
        true
    } catch (e: Throwable) {
        false
    }

    /** VirtualApp does not have BlackBox's Google-services problem, so GMS is always "supported". */
    @JvmStatic
    fun isGmsSupported(): Boolean = true

    /** Shared preferences used across the app. */
    @JvmStatic
    val mRemarkSharedPreferences: SharedPreferences by lazy {
        App.getContext().getSharedPreferences("UserRemark", Context.MODE_PRIVATE)
    }

    /** No-op: permissions are not requested in VirtualApp (user decision); the game handles its own. */
    @JvmStatic
    fun requestGamePermissions() {
        Log.d(TAG, "requestGamePermissions: VirtualApp does not request permissions; the game handles its own")
    }
}
