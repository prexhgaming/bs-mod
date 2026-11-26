package black.android.view;


import zyn.bs.core.reflection.annotation.BClassName;
import zyn.bs.core.reflection.annotation.BMethod;

@BClassName("android.view.DisplayAdjustments")
public interface DisplayAdjustments {
    @BMethod
    void setCompatibilityInfo();
}
