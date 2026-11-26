# ============================================================================
#  Release shrinking / obfuscation rules
# ============================================================================
#  Goal: keep exactly what the BlackBox engine needs at runtime, let R8 remove
#  (and rename) everything else - app code, unused library classes, dead SDK
#  paths. The previous rule set was "-keep class top.niunaijun.blackbox.** {*;}",
#  which pinned every engine class AND every member, so R8 could not remove
#  anything at all.
#
#  Two different needs inside the engine:
#    1. Class *names* must never change  -> reflection (Class.forName) + the
#       guest process resolving engine classes by name.
#    2. A small number of classes must never be *removed*, because the only
#       reference to them is a string.
#  (1) is handled with -keepnames (allows removal), (2) with -keep.
# ============================================================================

-keepattributes *Annotation*,Signature,InnerClasses,EnclosingMethod,MethodParameters
-renamesourcefileattribute SourceFile
-keepattributes SourceFile,LineNumberTable

# ----------------------------------------------------------------------------
# Engine: never rename anything, but let R8 drop unreachable code
# ----------------------------------------------------------------------------
-keepnames class top.niunaijun.blackbox.** { *; }
-keepnames class top.niunaijun.jnihook.** { *; }
-keepnames class top.niunaijun.blackreflection.** { *; }

# ----------------------------------------------------------------------------
# Engine: classes that are only reachable through a string (Class.forName)
#   BlackBoxCore.java:1418            -> top.niunaijun.blackbox.closecode.Entry
#   BaseInstrumentationDelegate.java:88 -> top.niunaijun.blackbox.utils.ResultsStorage
#   BResourcesManager.java:184        -> top.niunaijun.blackbox.BlackBoxCore
# ----------------------------------------------------------------------------
-keep class top.niunaijun.blackbox.BlackBoxCore { *; }
-keep class top.niunaijun.blackbox.utils.ResultsStorage { *; }
-keep class top.niunaijun.blackbox.closecode.** { *; }

# Guest-process entry points, started by name from the stub components.
-keep class top.niunaijun.blackbox.app.BActivityThread { *; }
-keep class top.niunaijun.blackbox.app.BRuntimeInit { *; }
-keep class top.niunaijun.blackbox.app.configuration.** { *; }

# Binder services / managers launched by name inside the service process.
-keep class top.niunaijun.blackbox.server.** { *; }
-keep class top.niunaijun.blackbox.core.system.** { *; }

# Hooks are installed reflectively per Android API level.
-keep class top.niunaijun.blackbox.fake.** { *; }

# ----------------------------------------------------------------------------
# AIDL: stubs are instantiated through the interface name in many IPC paths
# ----------------------------------------------------------------------------
-keep interface top.niunaijun.blackbox.**.I* { *; }
-keep class **$Stub { *; }
-keep class **$Stub$Proxy { *; }
-keep class android.**.I* { *; }

# ----------------------------------------------------------------------------
# Native bridge (JNI resolves these by name)
# ----------------------------------------------------------------------------
-keepclasseswithmembernames class * {
    native <methods>;
}
-keep class top.niunaijun.blackbox.utils.NativeUtils { *; }

# ----------------------------------------------------------------------------
# BlackReflection: generated mirror classes + annotations
# ----------------------------------------------------------------------------
-keep class mirror.** { *; }
-keep @top.niunaijun.blackreflection.annotation.BClass class * {*;}
-keep @top.niunaijun.blackreflection.annotation.BClassName class * {*;}
-keep @top.niunaijun.blackreflection.annotation.BClassNameNotProcess class * {*;}
-keepclasseswithmembernames class * {
    @top.niunaijun.blackreflection.annotation.BField.* <methods>;
    @top.niunaijun.blackreflection.annotation.BFieldNotProcess.* <methods>;
    @top.niunaijun.blackreflection.annotation.BFieldSetNotProcess.* <methods>;
    @top.niunaijun.blackreflection.annotation.BFieldCheckNotProcess.* <methods>;
    @top.niunaijun.blackreflection.annotation.BMethod.* <methods>;
    @top.niunaijun.blackreflection.annotation.BStaticField.* <methods>;
    @top.niunaijun.blackreflection.annotation.BStaticMethod.* <methods>;
    @top.niunaijun.blackreflection.annotation.BMethodCheckNotProcess.* <methods>;
    @top.niunaijun.blackreflection.annotation.BConstructor.* <methods>;
    @top.niunaijun.blackreflection.annotation.BConstructorNotProcess.* <methods>;
}

# ----------------------------------------------------------------------------
# App side
# ----------------------------------------------------------------------------
# Application class is named in the manifest.
-keep class top.niunaijun.blackboxa.app.App { *; }
# Crash viewer is started by an explicit Intent and lives in its own process.
-keep class top.niunaijun.blackboxa.crash.** { *; }
# ViewBinding: the inflate(...) factories are invoked reflectively by the
# `inflate()` delegate in util/ViewBindingEx.kt, so R8 must not remove or rename
# them. Without this every screen fails with NoSuchMethodException.
-keep class top.niunaijun.blackboxa.databinding.** { *; }

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
# Optional dependencies the engine checks for but does not bundle
# ----------------------------------------------------------------------------
-dontwarn org.osmdroid.**
-dontwarn com.google.android.gms.**
-dontwarn com.android.**
-dontwarn android.**
-dontwarn org.apache.**
-dontwarn com.moandjiezana.**
-dontwarn com.github.tiann.**
-dontwarn com.codinggay.**
-dontwarn top.niunaijun.**
-dontwarn javax.**
-dontwarn java.**
