package black.android.content.pm;

import android.os.IBinder;
import android.os.IInterface;

import zyn.bs.core.reflection.annotation.BClassName;
import zyn.bs.core.reflection.annotation.BStaticMethod;

@BClassName("mirror.android.content.pm.ILauncherApps")
public interface ILauncherApps {
    @BClassName("android.content.pm.ILauncherApps$Stub")
    interface Stub {
        @BStaticMethod
        IInterface asInterface(IBinder binder);
    }
}
