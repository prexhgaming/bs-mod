package black.android.view.accessibility;

import android.os.IBinder;
import android.os.IInterface;

import zyn.bs.core.reflection.annotation.BClassName;
import zyn.bs.core.reflection.annotation.BStaticMethod;

@BClassName("android.view.accessibility.IAccessibilityManager")
public interface IAccessibilityManager {
    @BClassName("android.view.accessibility.IAccessibilityManager$Stub")
    interface Stub {
        @BStaticMethod
        IInterface asInterface(IBinder IBinder0);
    }
}
