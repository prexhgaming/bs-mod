plugins {
    alias(libs.plugins.android.library)
    alias(libs.plugins.kotlin.android)
    alias(libs.plugins.ksp)
    alias(libs.plugins.kotlin.parcelize)
}

// The renjana engine, packaged as a library so a thin loader app can own the
// UI and the application id. Everything that used to live in an application
// module (applicationId, signing, ABI splits, the launcher activity) now lives
// in :loader — this module only ships code, resources and manifest entries.
android {
    namespace = "com.fesu.renjana"
    compileSdk = libs.versions.compileSdk.get().toInt()

    defaultConfig {
        minSdk = libs.versions.minSdk.get().toInt()
        targetSdk = libs.versions.targetSdk.get().toInt()

        vectorDrawables {
            useSupportLibrary = true
        }

        // Runtime-needed classes (Pine hooks, Room, reflection targets) must
        // survive R8 in the consuming app, so the rules ship with the library.
        consumerProguardFiles("proguard-rules.pro")
    }

    buildTypes {
        release {
            isMinifyEnabled = false
        }
    }

    compileOptions {
        sourceCompatibility = JavaVersion.VERSION_17
        targetCompatibility = JavaVersion.VERSION_17
    }
    kotlinOptions {
        jvmTarget = "17"
    }
    buildFeatures {
        compose = true
        buildConfig = true
    }
    composeOptions {
        kotlinCompilerExtensionVersion = libs.versions.composeCompiler.get()
    }
    packaging {
        resources {
            excludes += "/META-INF/{AL2.0,LGPL2.1}"
        }
    }
}

dependencies {
    // Core
    api(libs.androidx.core.ktx)
    api(libs.androidx.appcompat)
    api(libs.androidx.lifecycle.runtime.ktx)
    api(libs.androidx.lifecycle.viewmodel.ktx)
    api(libs.androidx.activity.compose)

    // Compose — the engine still carries its own screens, even though the
    // loader does not show them. api so they resolve for anyone reusing them.
    api(platform(libs.androidx.compose.bom))
    api(libs.androidx.compose.ui)
    api(libs.androidx.compose.graphics)
    api(libs.androidx.compose.preview)
    api(libs.androidx.compose.material3)
    api(libs.androidx.compose.material.icons.extended)
    api(libs.androidx.lifecycle.viewmodel.compose)
    api(libs.androidx.navigation.compose)

    // Hooking (Root) - Xposed API (compileOnly, not bundled at runtime).
    // Pine provides Xposed API compatibility at runtime via pine:xposed below.
    compileOnly(libs.xposed.api)

    // Hooking (Non-Root) - Pine framework (bundled, includes native libpine.so).
    // core: the hook engine (Pine.hook, PineConfig).
    api(libs.pine.core)
    // xposed: Xposed compatibility bridge — lets XC_MethodHook / XposedHelpers /
    // XposedBridge work transparently on top of Pine. Required because CoreHooks
    // uses Xposed callback types.
    api(libs.pine.xposed)

    // Google Play Services (compileOnly for virtualization)
    compileOnly(libs.play.services.auth)
    compileOnly(libs.play.services.base)

    // Database
    api(libs.androidx.room.runtime)
    api(libs.androidx.room.ktx)
    ksp(libs.androidx.room.compiler)

    // Utilities
    api(libs.gson)

    // Coroutines
    api(libs.kotlinx.coroutines.android)
}
