package zyn.bs.view.main

import android.content.Context
import android.util.Log
import com.lody.virtual.client.core.VirtualCore
import zyn.bs.app.App
import zyn.bs.biz.cache.AppSharedPreferenceDelegate

/**
 * Thin host-side wrapper around the VirtualApp engine.
 *
 * Engine boot and the guest-process lifecycle hook (which maps `libzyn-bs.so`)
 * live in [App.onCreate] via `AppCallback`. This class only keeps the host-side
 * preference flags and exposes the engine singleton.
 */
class BlackBoxLoader {

    private val prefsContext: Context
        get() = App.getContext()

    private var mHideRoot: Boolean by AppSharedPreferenceDelegate(prefsContext, false)
    private var mHideXposed: Boolean by AppSharedPreferenceDelegate(prefsContext, false)
    private var mDaemonEnable: Boolean by AppSharedPreferenceDelegate(prefsContext, false)
    private var mShowShortcutPermissionDialog: Boolean by AppSharedPreferenceDelegate(prefsContext, true)

    fun hideRoot(): Boolean = mHideRoot
    fun invalidHideRoot(value: Boolean) { mHideRoot = value }

    fun hideXposed(): Boolean = mHideXposed
    fun invalidHideXposed(value: Boolean) { mHideXposed = value }

    fun daemonEnable(): Boolean = mDaemonEnable
    fun invalidDaemonEnable(value: Boolean) { mDaemonEnable = value }

    fun showShortcutPermissionDialog(): Boolean = mShowShortcutPermissionDialog
    fun invalidShortcutPermissionDialog(value: Boolean) { mShowShortcutPermissionDialog = value }

    /** Returns the VirtualApp engine singleton (replaces `BlackBoxCore.get()`). */
    fun getVirtualCore(): VirtualCore = VirtualCore.get()

    /** No-op: engine init happens in [App.onCreate]. */
    fun attachBaseContext(context: Context) {
        Log.d(TAG, "attachBaseContext: no-op, VirtualCore already initialised")
    }

    /** No-op: the guest lifecycle hook is registered in [App.onCreate]. */
    fun addLifecycleCallback() {
        Log.d(TAG, "addLifecycleCallback: no-op, AppCallback registered in App")
    }

    /** No-op: engine init happens in [App.onCreate]. */
    fun doOnCreate(context: Context) {
        Log.d(TAG, "doOnCreate: no-op, VirtualCore already initialised")
    }

    companion object {
        val TAG: String = BlackBoxLoader::class.java.simpleName
    }
}
