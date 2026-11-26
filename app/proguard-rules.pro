# ============================================================================
#  Release shrinking / obfuscation rules
# ============================================================================
#  The engine is VirtualApp (:VirtualApp — com.lody.virtual.** plus the vendored
#  com.xdja.** helpers). It is kept whole on purpose: it resolves its own stub
#  components, AIDL stubs and hook proxies by name at runtime, so a single
#  renamed class surfaces as a ClassNotFoundException deep inside a sandboxed
#  guest process, far away from the actual cause.
#
#  App code, ViewBinding factories and unused libraries are still shrunk and
#  renamed normally.
# ============================================================================

-keepattributes *Annotation*,Signature,InnerClasses,EnclosingMethod,MethodParameters
-renamesourcefileattribute SourceFile
-keepattributes SourceFile,LineNumberTable

# ----------------------------------------------------------------------------
# Engine: VirtualApp
# ----------------------------------------------------------------------------
-keep class com.lody.virtual.** { *; }
-keep interface com.lody.virtual.** { *; }
-keep class com.xdja.** { *; }

# ----------------------------------------------------------------------------
# AIDL: stubs are instantiated through the interface name in many IPC paths
# ----------------------------------------------------------------------------
-keep class **$Stub { *; }
-keep class **$Stub$Proxy { *; }
-keep class android.**.I* { *; }

# ----------------------------------------------------------------------------
# Native bridge (JNI resolves these by name)
# ----------------------------------------------------------------------------
-keepclasseswithmembernames class * {
    native <methods>;
}

# ----------------------------------------------------------------------------
# App side
# ----------------------------------------------------------------------------
# Application class is named in the manifest.
-keep class zyn.bs.app.App { *; }
# ViewBinding: the inflate(...) factories are invoked reflectively by the
# `inflate()` delegate in util/ViewBindingEx.kt, so R8 must not remove or rename
# them. Without this every screen fails with NoSuchMethodException.
-keep class zyn.bs.databinding.** { *; }

# ----------------------------------------------------------------------------
# Libraries that rely on reflection / generics
# ----------------------------------------------------------------------------
-keep class kotlin.Metadata { *; }
-keepclassmembers class **$WhenMappings { <fields>; }
-keep class kotlinx.coroutines.** { *; }
-dontwarn kotlinx.coroutines.**
-keep class com.afollestad.materialdialogs.** { *; }
-dontwarn com.afollestad.materialdialogs.**

# ----------------------------------------------------------------------------
# Optional / framework classes the engine only touches reflectively.
# Nothing here is bundled; R8 must not fail because the references are absent.
# ----------------------------------------------------------------------------
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
