package zyn.bs

import android.content.Context
import android.content.pm.PackageInfo
import android.graphics.drawable.Drawable
import androidx.core.content.pm.PackageInfoCompat

/**
 * The single game this loader exists for.
 *
 * Everything the launcher needs is read from the package manager — the display
 * name is never hardcoded, so a rename or a localised build shows correctly.
 * Nothing here touches the container: this is host-side resolution only.
 */
object GameTarget {

    /** Target game package (Knives Out, NetEase). */
    const val PACKAGE_NAME: String = "com.netease.newspike"

    /**
     * The injected mod's library name, without the `lib` prefix or `.so` suffix.
     *
     * Lives here next to [PACKAGE_NAME] so a fork only edits this one file: change
     * the package to retarget a different game, and the library name if you rename
     * the `LOCAL_MODULE` in `loader/src/main/cpp/Android.mk`. Must match it —
     * `System.loadLibrary` resolves `lib<name>.so` from the APK's native lib dir.
     */
    const val MOD_LIBRARY: String = "zyn-bs"

    /** Everything needed to create a container instance for the target. */
    data class Resolved(
        val label: String,
        val packageName: String,
        val versionName: String,
        val versionCode: Int,
        val apkPath: String
    )

    /**
     * Resolves the target from the host package manager.
     *
     * Returns null when the game is not installed — that is the `MISSING` state
     * on screen, not an error: the user can still import an APK by hand.
     */
    fun resolve(context: Context): Resolved? {
        val pm = context.packageManager
        val info = runCatching {
            pm.getApplicationInfo(PACKAGE_NAME, 0)
        }.getOrNull() ?: return null

        val pkg = runCatching { pm.getPackageInfo(PACKAGE_NAME, 0) }.getOrNull()
        val label = runCatching { pm.getApplicationLabel(info).toString() }
            .getOrNull()
            ?.takeIf { it.isNotBlank() }
            ?: PACKAGE_NAME

        return Resolved(
            label = label,
            packageName = PACKAGE_NAME,
            versionName = pkg?.versionName.orEmpty(),
            versionCode = pkg?.let { PackageInfoCompat.getLongVersionCode(it).toInt() } ?: 0,
            apkPath = info.sourceDir
        )
    }

    /** The game's icon, or null when it is not installed / has no icon. */
    fun icon(context: Context): Drawable? =
        runCatching { context.packageManager.getApplicationIcon(PACKAGE_NAME) }.getOrNull()

    /** Reads package metadata for a hand-picked APK (the import path). */
    fun readArchive(context: Context, apkPath: String): Resolved? {
        val pm = context.packageManager
        val pkg: PackageInfo = runCatching {
            pm.getPackageArchiveInfo(apkPath, 0)
        }.getOrNull() ?: return null

        val appInfo = pkg.applicationInfo ?: return null
        // loadLabel/loadIcon need the source dir set on the ApplicationInfo,
        // otherwise they resolve against the host app and return garbage.
        appInfo.sourceDir = apkPath
        appInfo.publicSourceDir = apkPath

        val label = runCatching { appInfo.loadLabel(pm).toString() }
            .getOrNull()
            ?.takeIf { it.isNotBlank() }
            ?: pkg.packageName

        return Resolved(
            label = label,
            packageName = pkg.packageName,
            versionName = pkg.versionName.orEmpty(),
            versionCode = PackageInfoCompat.getLongVersionCode(pkg).toInt(),
            apkPath = apkPath
        )
    }

    /** True when the package manager can see the game on this device. */
    fun isInstalled(context: Context): Boolean = resolve(context) != null
}
