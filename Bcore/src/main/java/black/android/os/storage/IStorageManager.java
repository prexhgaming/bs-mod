package black.android.os.storage;

import android.os.IBinder;
import android.os.IInterface;

import zyn.bs.core.reflection.annotation.BClassName;
import zyn.bs.core.reflection.annotation.BStaticMethod;

@BClassName("android.os.storage.IStorageManager")
public interface IStorageManager {
    @BClassName("android.os.storage.IStorageManager$Stub")
    interface Stub {
        @BStaticMethod
        IInterface asInterface(IBinder IBinder0);
    }
}
