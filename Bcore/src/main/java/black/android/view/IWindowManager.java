package black.android.view;

import android.os.IBinder;
import android.os.IInterface;

import zyn.bs.core.reflection.annotation.BClassName;
import zyn.bs.core.reflection.annotation.BStaticMethod;

@BClassName("android.view.IWindowManager")
public interface IWindowManager {
    @BClassName("android.view.IWindowManager$Stub")
    interface Stub {
        @BStaticMethod
        IInterface asInterface(IBinder IBinder0);
    }
}
