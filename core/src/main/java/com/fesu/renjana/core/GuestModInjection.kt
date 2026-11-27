package com.fesu.renjana.core

import android.content.Context
import com.fesu.renjana.utils.RenjanaLog

/**
 * Seam between the container and the app that hosts it.
 *
 * `:core` must not reference `:loader` — that would be a circular dependency and
 * would also hardcode one app's library name into the engine. Instead the loader
 * registers an [Installer] at startup and [StubActivity] calls [install] at the
 * same lifecycle point VirtualApp's `AppCallback.afterApplicationCreate` used:
 * after the guest `Application.onCreate()`, before any guest Activity exists.
 *
 * Registration is per process. Android constructs the app's `Application` class
 * once in every process, including the guest processes `:p0`–`:p9`, so the
 * installer is in place before any `StubActivity` runs there.
 */
object GuestModInjection {

    fun interface Installer {
        /**
         * Called in the guest process, once per launch attempt.
         *
         * Implementations must be cheap and must never throw: a broken mod has to
         * degrade to "the game runs without it", not to a dead guest launch.
         */
        fun install(context: Context, guestPackageName: String)
    }

    @Volatile
    private var installer: Installer? = null

    /** Registers the host app's installer. Safe to call more than once. */
    fun register(installer: Installer) {
        this.installer = installer
    }

    /**
     * Invokes the registered installer, if any. A no-op when the host app never
     * registered one, which keeps `:core` usable on its own (its own MainActivity
     * still runs without any mod).
     */
    fun install(context: Context, guestPackageName: String) {
        try {
            installer?.install(context, guestPackageName)
        } catch (t: Throwable) {
            RenjanaLog.w("GuestModInjection", "installer failed for $guestPackageName: ${t.message}")
        }
    }
}
