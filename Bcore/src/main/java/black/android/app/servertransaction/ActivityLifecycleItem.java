package black.android.app.servertransaction;


import zyn.bs.core.reflection.annotation.BClassName;
import zyn.bs.core.reflection.annotation.BMethod;

@BClassName("android.app.servertransaction.ActivityLifecycleItem")
public interface ActivityLifecycleItem {
    @BMethod
    Integer getTargetState();
}
