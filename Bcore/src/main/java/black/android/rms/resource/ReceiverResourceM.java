package black.android.rms.resource;


import zyn.bs.core.reflection.annotation.BClassName;
import zyn.bs.core.reflection.annotation.BField;

@BClassName("android.rms.resource.ReceiverResource")
public interface ReceiverResourceM {
    @BField
    String[] mWhiteList();
}
