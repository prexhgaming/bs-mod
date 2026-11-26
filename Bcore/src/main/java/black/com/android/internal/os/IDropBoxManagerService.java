package black.com.android.internal.os;

import android.os.IBinder;
import android.os.IInterface;

import zyn.bs.core.reflection.annotation.BClassName;
import zyn.bs.core.reflection.annotation.BStaticMethod;

@BClassName("com.android.internal.os.IDropBoxManagerService")
public interface IDropBoxManagerService {
    @BClassName("com.android.internal.os.IDropBoxManagerService$Stub")
    interface Stub {
        @BStaticMethod
        IInterface asInterface(IBinder IBinder0);
    }
}
