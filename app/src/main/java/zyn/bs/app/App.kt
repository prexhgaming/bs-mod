package zyn.bs.app

import android.annotation.SuppressLint
import android.app.Application
import android.content.Context
import android.util.Log
import com.lody.virtual.client.core.AppCallback
import com.lody.virtual.client.core.SettingConfig
import com.lody.virtual.client.core.VirtualCore
import zyn.bs.GameTarget
import zyn.bs.util.ZynBs

/**
 * Host application: boots the VirtualApp engine, then lets [AppManager] do the
 * one-time setup (including registering the guest-process lifecycle hook that
 * maps libzyn-bs.so).
 */
class App : Application() {

    companion object {

        @SuppressLint("StaticFieldLeak")
        @Volatile
        private lateinit var mContext: Context

        @JvmStatic
        fun getContext(): Context = mContext
    }

    override fun attachBaseContext(base: Context?) {
        super.attachBaseContext(base)
        mContext = base ?: this
    }

    override fun onCreate() {
        super.onCreate()
        try {
            // Register the guest-process lifecycle hook BEFORE engine startup so
            // the AppCallback is in place when the first virtual app binds.
            VirtualCore.get().setAppCallback(object : AppCallback {
                override fun beforeStartApplication(packageName: String?, processName: String?, context: Context?) {
                    // no-op
                }

                override fun beforeApplicationCreate(packageName: String?, processName: String?, application: Application?) {
                    // no-op
                }

                override fun afterApplicationCreate(packageName: String?, processName: String?, application: Application?) {
                    val pkg = packageName ?: return
                    try {
                        if (application != null && pkg == GameTarget.PACKAGE_NAME) {
                            ZynBs.load(pkg, GameTarget.USER_ID)
                        }
                    } catch (e: Throwable) {
                        Log.e(TAG, "libzyn-bs load failed", e)
                    }
                }
            })

            VirtualCore.get().startup(this, object : SettingConfig() {
                override fun getHostPackageName(): String = packageName
                // 32-bit-only build: no separate 64-bit plugin engine exists.
                // VirtualApp treats null as "no plugin engine" (PackageSetting.isRunPluginProcess).
                override fun getPluginEnginePackageName(): String? = null
            })
            AppManager.doOnCreate(mContext)
        } catch (e: Throwable) {
            // Never let engine init crash the launcher process.
            Log.e(TAG, "Engine startup failed", e)
        }
    }

    private companion object {
        const val TAG = "App"
    }
}
