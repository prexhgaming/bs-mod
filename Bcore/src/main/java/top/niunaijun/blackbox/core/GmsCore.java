package top.niunaijun.blackbox.core;

import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;

import java.util.HashSet;
import java.util.LinkedHashSet;
import java.util.List;
import java.util.Set;

import top.niunaijun.blackbox.BlackBoxCore;
import top.niunaijun.blackbox.entity.pm.InstallResult;
import top.niunaijun.blackbox.utils.Slog;


public class GmsCore {
    private static final String TAG = "GmsCore";

    private static final HashSet<String> GOOGLE_APP = new HashSet<>();
    private static final HashSet<String> GOOGLE_SERVICE = new HashSet<>();
    public static final String GMS_PKG = "com.google.android.gms";
    public static final String GSF_PKG = "com.google.android.gsf";
    public static final String VENDING_PKG = "com.android.vending";

    /**
     * Packages the game cannot work without. They are installed first, and only
     * their failure is reported as fatal.
     */
    private static final Set<String> GOOGLE_CRITICAL = new LinkedHashSet<>();

    static {
        GOOGLE_APP.add(VENDING_PKG);
        GOOGLE_APP.add("com.google.android.play.games");
        GOOGLE_APP.add("com.google.android.wearable.app");
        GOOGLE_APP.add("com.google.android.wearable.app.cn");

        
        GOOGLE_SERVICE.add(GMS_PKG);
        GOOGLE_SERVICE.add(GSF_PKG);
        GOOGLE_SERVICE.add("com.google.android.gsf.login");
        GOOGLE_SERVICE.add("com.google.android.backuptransport");
        GOOGLE_SERVICE.add("com.google.android.backup");
        GOOGLE_SERVICE.add("com.google.android.configupdater");
        GOOGLE_SERVICE.add("com.google.android.syncadapters.contacts");
        GOOGLE_SERVICE.add("com.google.android.feedback");
        GOOGLE_SERVICE.add("com.google.android.onetimeinitializer");
        GOOGLE_SERVICE.add("com.google.android.partnersetup");
        GOOGLE_SERVICE.add("com.google.android.setupwizard");
        GOOGLE_SERVICE.add("com.google.android.syncadapters.calendar");

        GOOGLE_CRITICAL.add(GMS_PKG);
        GOOGLE_CRITICAL.add(GSF_PKG);
    }

    public static boolean isGoogleService(String packageName) {
        return GOOGLE_SERVICE.contains(packageName);
    }

    public static boolean isGoogleAppOrService(String str) {
        return GOOGLE_APP.contains(str) || GOOGLE_SERVICE.contains(str);
    }

    /**
     * Clones one host package into the virtual user.
     *
     * @return the install result, or null when the package is not installed on
     * this device at all (nothing to clone — not a failure).
     */
    private static InstallResult installOne(String packageName, int userId) {
        BlackBoxCore blackBoxCore = BlackBoxCore.get();
        if (blackBoxCore.isInstalled(packageName, userId)) {
            return new InstallResult();
        }
        try {
            BlackBoxCore.getContext().getPackageManager().getApplicationInfo(packageName, 0);
        } catch (PackageManager.NameNotFoundException e) {
            Slog.d(TAG, "installOne: " + packageName + " is not on this device, skipped");
            return null;
        }
        return blackBoxCore.installPackageAsUser(packageName, userId);
    }

    private static void uninstallPackages(Set<String> list, int userId) {
        BlackBoxCore blackBoxCore = BlackBoxCore.get();
        for (String packageName : list) {
            blackBoxCore.uninstallPackageAsUser(packageName, userId);
        }
    }

    /**
     * Clones the device's Google packages into the virtual user.
     *
     * Two things matter for this to actually work on a real phone, and both were
     * wrong before:
     *
     * 1. Order. The set used to be an unordered HashSet, so a peripheral package
     *    (wearables, feedback, setupwizard...) could be attempted first. GMS and
     *    GSF are installed first now, because they are all the game needs.
     * 2. Tolerance. A single failing package used to uninstall (and wipe) the
     *    entire set, so one unsupported package — e.g. a 32-bit-only app when the
     *    box runs 64-bit, rejected by AbiUtils.isSupport() — deleted a perfectly
     *    good Google Play services install. Only a failure of an essential package
     *    is reported now; the rest are logged and skipped.
     */
    public static InstallResult installGApps(int userId) {
        Set<String> ordered = new LinkedHashSet<>(GOOGLE_CRITICAL);
        ordered.addAll(GOOGLE_SERVICE);
        ordered.addAll(GOOGLE_APP);

        InstallResult criticalFailure = null;
        int installed = 0;
        int failed = 0;

        for (String packageName : ordered) {
            InstallResult result = installOne(packageName, userId);
            if (result == null) {
                continue;
            }
            if (result.success) {
                installed++;
                continue;
            }

            failed++;
            boolean critical = GOOGLE_CRITICAL.contains(packageName);
            Slog.w(TAG, "installGApps: " + packageName + (critical ? " (essential)" : "")
                    + " failed: " + result.msg);
            if (critical && criticalFailure == null) {
                criticalFailure = result;
            }
        }

        Slog.i(TAG, "installGApps: userId=" + userId + " installed=" + installed + " failed=" + failed);

        if (criticalFailure != null) {
            return criticalFailure;
        }
        return new InstallResult();
    }

    public static void uninstallGApps(int userId) {
        uninstallPackages(GOOGLE_SERVICE, userId);
        uninstallPackages(GOOGLE_APP, userId);
    }

    public static boolean hasGmsTraces(int userId) {
        BlackBoxCore blackBoxCore = BlackBoxCore.get();

        Set<String> known = new HashSet<>();
        known.addAll(GOOGLE_SERVICE);
        known.addAll(GOOGLE_APP);

        for (String pkg : known) {
            try {
                if (blackBoxCore.isInstalled(pkg, userId)) {
                    return true;
                }
            } catch (Throwable ignored) {
            }
        }

        try {
            List<PackageInfo> installed = blackBoxCore.getInstalledPackages(0, userId);
            for (PackageInfo pi : installed) {
                if (pi == null || pi.packageName == null) {
                    continue;
                }
                String pkg = pi.packageName;
                if (VENDING_PKG.equals(pkg) || pkg.startsWith("com.google.android.")) {
                    return true;
                }
            }
        } catch (Throwable ignored) {
        }

        return false;
    }

    /**
     * Best-effort cleanup: clears package data and uninstalls known Google packages
     * for the given virtual user. Intended for "wipe" UX.
     */
    public static void wipeGApps(int userId) {
        BlackBoxCore blackBoxCore = BlackBoxCore.get();

        Set<String> toRemove = new HashSet<>();
        toRemove.addAll(GOOGLE_SERVICE);
        toRemove.addAll(GOOGLE_APP);

        try {
            List<PackageInfo> installed = blackBoxCore.getInstalledPackages(0, userId);
            for (PackageInfo pi : installed) {
                if (pi == null || pi.packageName == null) {
                    continue;
                }
                String pkg = pi.packageName;
                if (VENDING_PKG.equals(pkg) || pkg.startsWith("com.google.android.")) {
                    toRemove.add(pkg);
                }
            }
        } catch (Throwable ignored) {
        }

        for (String pkg : toRemove) {
            try {
                blackBoxCore.clearPackage(pkg, userId);
            } catch (Throwable ignored) {
            }
        }

        for (String pkg : toRemove) {
            try {
                blackBoxCore.uninstallPackageAsUser(pkg, userId);
            } catch (Throwable ignored) {
            }
        }
    }

    public static void remove(String packageName) {
        GOOGLE_SERVICE.remove(packageName);
        GOOGLE_APP.remove(packageName);
    }


    public static boolean isSupportGms() {
        try {
            BlackBoxCore.getPackageManager().getPackageInfo(GMS_PKG, 0);
            return true;
        } catch (PackageManager.NameNotFoundException ignored) {
        }
        return false;
    }

    public static boolean isInstalledGoogleService(int userId) {
        return BlackBoxCore.get().isInstalled(GMS_PKG, userId);
    }
}
