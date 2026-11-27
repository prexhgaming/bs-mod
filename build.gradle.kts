// Top-level build file. Plugin versions come from gradle/libs.versions.toml, so
// AGP / Kotlin / KSP are bumped in exactly one place.
plugins {
    alias(libs.plugins.android.application) apply false
    alias(libs.plugins.android.library) apply false
    alias(libs.plugins.kotlin.android) apply false
    alias(libs.plugins.ksp) apply false
}

tasks.register<Delete>("clean") {
    delete(rootProject.layout.buildDirectory)
}
