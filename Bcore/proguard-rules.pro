# Add project specific ProGuard rules here.
# You can control the set of applied configuration files using the
# proguardFiles setting in build.gradle.
#
# For more details, see
#   http://developer.android.com/guide/developing/tools/proguard.html

# If your project uses WebView with JS, uncomment the following
# and specify the fully qualified class name to the JavaScript interface
# class:
#-keepclassmembers class fqcn.of.javascript.interface.for.webview {
#   public *;
#}

# Uncomment this to preserve the line number information for
# debugging stack traces.
#-keepattributes SourceFile,LineNumberTable

# If you keep the line number information, uncomment this to
# hide the original source file name.
#-renamesourcefileattribute SourceFile

-keep class zyn.bs.core.** {*; }
-keep class zyn.bs.core.jnihook.** {*; }
-keep class mirror.** {*; }
-keep class android.** {*; }
-keep class com.android.** {*; }

-keep class zyn.bs.core.reflection.** {*; }
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