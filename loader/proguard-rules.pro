# Loader ProGuard rules.
#
# Most of what must survive R8 is declared by :core itself — core/build.gradle
# sets `consumerProguardFiles 'proguard-rules.pro'`, and those rules keep the stub
# activities, hook callbacks, Pine, Room entities, models and GMS classes that are
# only ever reached reflectively. Do not duplicate them here; add to that file if
# the engine needs more.
#
# This file only covers the loader's own surface.

# The mod installer is registered through a core seam and loaded via
# System.loadLibrary, so keep the class and its JNI-visible name intact.
-keep class zyn.bs.ModLoader { *; }

# The target resolution is the only place that names the game package and the
# library; keeping it readable makes a stack trace from the field actionable.
-keep class zyn.bs.GameTarget { *; }

# Native methods are resolved by name from C++; never strip or rename them.
-keepclasseswithmembernames class * {
    native <methods>;
}

# Entry point referenced from the manifest and from view binding.
-keep class zyn.bs.LauncherActivity { *; }
-keep class zyn.bs.LoaderApp { *; }

# View binding classes are generated and referenced only from our code; R8 can
# shrink unused ones, but the binding for the launcher layout must stay.
-keep class zyn.bs.databinding.** { *; }

# Keep annotation metadata that Room/AppCompat read at runtime.
-keepattributes *Annotation*, Signature, InnerClasses, EnclosingMethod

# The engine reaches AndroidX internals it cannot see at compile time.
-dontwarn androidx.**

# These are compileOnly in :core (de.robv.android.xposed:api, play-services-*):
# referenced by the hook code, absent from the runtime classpath. Without these
# R8 fails the release build with "Missing class ...", not a runtime error.
-dontwarn de.robv.android.xposed.**
-dontwarn com.google.android.gms.**
-dontwarn com.google.android.play.core.**
-dontwarn org.apache.**
-dontwarn org.slf4j.**
