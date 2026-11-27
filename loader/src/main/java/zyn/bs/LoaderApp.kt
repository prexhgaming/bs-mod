package zyn.bs

import com.fesu.renjana.RenjanaApplication

/**
 * The loader's [android.app.Application].
 *
 * It extends [RenjanaApplication] rather than replacing it. That class is what
 * publishes the engine singleton (`RenjanaApplication.get()`), installs the
 * crash handler, initialises Pine, `GuestInfoCache`, `DeviceFingerprint` and the
 * Google sign-in virtualizer, and warms the instance caches up from Room.
 * A plain `Application` here would leave every one of those uninitialised and
 * the container would fail at launch time, not at build time.
 */
class LoaderApp : RenjanaApplication() {

    override fun onCreate() {
        super.onCreate()
        // Must run before any StubActivity: this is what tells :core who loads
        // the mod into a guest process. Application.onCreate runs once per
        // process, including the guest processes (:p0..:p9), so the installer is
        // already registered by the time a stub launches there.
        ModLoader.register()
    }
}
