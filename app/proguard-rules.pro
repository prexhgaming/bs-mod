-keepattributes *Annotation*,Signature,InnerClasses,EnclosingMethod,MethodParameters
-renamesourcefileattribute SourceFile
-keepattributes SourceFile,LineNumberTable

# Engine: VirtualApp
-keep class com.lody.virtual.** { *; }
-keep interface com.lody.virtual.** { *; }
-keep class com.xdja.** { *; }

# AIDL: stubs are instantiated through the interface name in many IPC paths
-keep class **$Stub { *; }
-keep class **$Stub$Proxy { *; }
-keep class android.**.I* { *; }

# Native bridge (JNI resolves these by name)
-keepclasseswithmembernames class * {
    native <methods>;
}

# Keep the engine's native entry point – R8 must not strip it or rename it,
# otherwise the sandboxed guest process cannot launch (ClassNotFoundException
# deep inside the guest with no visible crash message).
-keep class com.lody.virtual.client.NativeEngine { *; }

# Keep the @Keep annotation used throughout the engine so that methods
# annotated with @Keep are not removed or obfuscated.
-keep @interface com.lody.virtual.helper.Keep { *; }
-keep @com.lody.virtual.helper.Keep class ** {
    public <methods>;
}

# Mirror classes – the engine uses mirror.* to avoid runtime reflection;
# these must be kept so R8 does not rename them away.
-keep @interface mirror.** { *; }
-keepclassmembers class mirror.** {
    public *;
}

# Parcelable / Serializable contracts used by the engine
-keepclassmembers class * implements android.os.Parcelable {
    public static final android.os.Parcelable$Creator *;
}
-keepclassmembers class * implements java.io.Serializable { *; }

# App side
# Application class is named in the manifest.
-keep class zyn.bs.app.App { *; }
# ViewBinding: the inflate(...) factories are invoked reflectively by the
# `inflate()` delegate in util/ViewBindingEx.kt, so R8 must not remove or rename
# them. Without this every screen fails with NoSuchMethodException.
-keep class zyn.bs.databinding.** { *; }

# Libraries that rely on reflection / generics
-keep class kotlin.Metadata { *; }
-keepclassmembers class **$WhenMappings { <fields>; }
-keep class kotlinx.coroutines.** { *; }
-dontwarn kotlinx.coroutines.**
-keep class com.afollestad.materialdialogs.** { *; }
-dontwarn com.afollestad.materialdialogs.**

# Optional / framework classes the engine only touches reflectively.
# Nothing here is bundled; R8 must not fail because the references are absent.
-dontwarn org.osmdroid.**
-dontwarn com.google.android.gms.**
-dontwarn com.android.**
-dontwarn com.android.internal.**
-dontwarn android.**
-dontwarn dalvik.**
-dontwarn libcore.**
-dontwarn org.apache.**
-dontwarn com.moandjiezana.**
-dontwarn com.github.tiann.**
-dontwarn com.codinggay.**
-dontwarn zyn.bs.**
-dontwarn javax.**
-dontwarn java.**