# ============================================================================
#  Consumer rules shipped with the :Bcore engine.
#  Any app that embeds this SDK gets these automatically.
# ============================================================================
#  Keep the engine's *names* (it resolves its own classes by name in the guest
#  process and through reflection), but do not pin every member: R8 is still
#  allowed to remove code that is genuinely unreachable, which is what keeps
#  the APK small.
# ============================================================================

-keepattributes *Annotation*,Signature,InnerClasses,EnclosingMethod

# Never rename engine internals (Class.forName / guest-process lookups).
-keepnames class top.niunaijun.blackbox.** { *; }
-keepnames class top.niunaijun.jnihook.** { *; }
-keepnames class top.niunaijun.blackreflection.** { *; }

# Only reachable through a string -> must not be removed.
-keep class top.niunaijun.blackbox.BlackBoxCore { *; }
-keep class top.niunaijun.blackbox.utils.ResultsStorage { *; }
-keep class top.niunaijun.blackbox.closecode.** { *; }

# Guest-process entry points.
-keep class top.niunaijun.blackbox.app.BActivityThread { *; }
-keep class top.niunaijun.blackbox.app.BRuntimeInit { *; }
-keep class top.niunaijun.blackbox.app.configuration.** { *; }

# Binder services / system managers started by name.
-keep class top.niunaijun.blackbox.server.** { *; }
-keep class top.niunaijun.blackbox.core.system.** { *; }

# Hooks installed reflectively per API level.
-keep class top.niunaijun.blackbox.fake.** { *; }

# AIDL stubs + proxies.
-keep interface top.niunaijun.blackbox.**.I* { *; }
-keep class **$Stub { *; }
-keep class **$Stub$Proxy { *; }

# JNI.
-keepclasseswithmembernames class * {
    native <methods>;
}
-keep class top.niunaijun.blackbox.utils.NativeUtils { *; }

# BlackReflection.
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

# Optional integrations the engine probes for but does not bundle.
-dontwarn org.osmdroid.**
-dontwarn com.google.android.gms.**
-dontwarn com.android.**
-dontwarn android.**
-dontwarn org.apache.**
-dontwarn top.niunaijun.**
