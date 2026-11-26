# ============================================================================
#  Release shrinking / obfuscation rules
# ============================================================================
#  Goal: keep exactly what the BlackBox engine needs at runtime, let R8 remove
#  (and rename) everything else - app code, unused library classes, dead SDK
#  paths. The previous rule set was "-keep class zyn.bs.core.** {*;}",
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
-keepnames class zyn.bs.core.** { *; }
-keepnames class zyn.bs.core.jnihook.** { *; }
-keepnames class zyn.bs.core.reflection.** { *; }

# ----------------------------------------------------------------------------
# Engine: classes that are only reachable through a string (Class.forName)
#   BlackBoxCore.java:1418            -> zyn.bs.core.closecode.Entry
#   BaseInstrumentationDelegate.java:88 -> zyn.bs.core.utils.ResultsStorage
#   BResourcesManager.java:184        -> zyn.bs.core.BlackBoxCore
# ----------------------------------------------------------------------------
-keep class zyn.bs.core.BlackBoxCore { *; }
-keep class zyn.bs.core.utils.ResultsStorage { *; }
-keep class zyn.bs.core.closecode.** { *; }

# Guest-process entry points, started by name from the stub components.
-keep class zyn.bs.core.app.BActivityThread { *; }
-keep class zyn.bs.core.app.BRuntimeInit { *; }
-keep class zyn.bs.core.app.configuration.** { *; }

# Binder services / managers launched by name inside the service process.
-keep class zyn.bs.core.server.** { *; }
-keep class zyn.bs.core.core.system.** { *; }

# Hooks are installed reflectively per Android API level.
-keep class zyn.bs.core.fake.** { *; }

# ----------------------------------------------------------------------------
# AIDL: stubs are instantiated through the interface name in many IPC paths
# ----------------------------------------------------------------------------
-keep interface zyn.bs.core.**.I* { *; }
-keep class **$Stub { *; }
-keep class **$Stub$Proxy { *; }
-keep class android.**.I* { *; }

# ----------------------------------------------------------------------------
# Native bridge (JNI resolves these by name)
# ----------------------------------------------------------------------------
-keepclasseswithmembernames class * {
    native <methods>;
}
-keep class zyn.bs.core.utils.NativeUtils { *; }

# ----------------------------------------------------------------------------
# BlackReflection: generated mirror classes + annotations
# ----------------------------------------------------------------------------
-keep class mirror.** { *; }
-keep @zyn.bs.core.reflection.annotation.BClass class * {*;}
-keep @zyn.bs.core.reflection.annotation.BClassName class * {*;}
-keep @zyn.bs.core.reflection.annotation.BClassNameNotProcess class * {*;}
-keepclasseswithmembernames class * {
    @zyn.bs.core.reflection.annotation.BField.* <methods>;
    @zyn.bs.core.reflection.annotation.BFieldNotProcess.* <methods>;
    @zyn.bs.core.reflection.annotation.BFieldSetNotProcess.* <methods>;
    @zyn.bs.core.reflection.annotation.BFieldCheckNotProcess.* <methods>;
    @zyn.bs.core.reflection.annotation.BMethod.* <methods>;
    @zyn.bs.core.reflection.annotation.BStaticField.* <methods>;
    @zyn.bs.core.reflection.annotation.BStaticMethod.* <methods>;
    @zyn.bs.core.reflection.annotation.BMethodCheckNotProcess.* <methods>;
    @zyn.bs.core.reflection.annotation.BConstructor.* <methods>;
    @zyn.bs.core.reflection.annotation.BConstructorNotProcess.* <methods>;
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
-dontwarn zyn.bs.**
-dontwarn javax.**
-dontwarn java.**
