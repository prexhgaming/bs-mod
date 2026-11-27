package com.fesu.renjana.utils

import android.content.Context
import java.util.UUID

/**
 * Utility functions for Renjana
 */
object Utils {

    /**
     * Version name of the running app, e.g. "4.0.1".
     *
     * Read from PackageManager instead of BuildConfig: `:core` is a library
     * module and AGP does not generate VERSION_NAME for libraries. Resolving it
     * at runtime also keeps the value honest for whichever app embeds this
     * library, rather than freezing a number into the engine.
     */
    fun appVersionName(context: Context): String {
        return try {
            context.packageManager
                .getPackageInfo(context.packageName, 0)
                .versionName
                ?: "unknown"
        } catch (e: Exception) {
            "unknown"
        }
    }
    
    /**
     * Generate a unique ID
     */
    fun generateId(): String {
        return UUID.randomUUID().toString()
    }

    /**
     * Format timestamp to readable string
     */
    fun formatTimestamp(timestamp: Long): String {
        val dateFormat = java.text.SimpleDateFormat("yyyy-MM-dd HH:mm:ss", java.util.Locale.getDefault())
        return dateFormat.format(java.util.Date(timestamp))
    }

    /**
     * Check if a string is a valid package name
     */
    fun isValidPackageName(packageName: String): Boolean {
        return packageName.matches(Regex("^[a-zA-Z][a-zA-Z0-9_]*(\\.[a-zA-Z][a-zA-Z0-9_]*)*$"))
    }

    /**
     * Get human-readable file size
     */
    fun formatFileSize(bytes: Long): String {
        val sizes = arrayOf("B", "KB", "MB", "GB")
        var size = bytes.toDouble()
        var index = 0
        while (size >= 1024 && index < sizes.size - 1) {
            size /= 1024
            index++
        }
        return String.format("%.2f %s", size, sizes[index])
    }
}
