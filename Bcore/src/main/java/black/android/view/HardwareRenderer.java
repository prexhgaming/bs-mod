package black.android.view;

import java.io.File;

import zyn.bs.core.reflection.annotation.BClassName;
import zyn.bs.core.reflection.annotation.BStaticMethod;

@BClassName("android.view.HardwareRenderer")
public interface HardwareRenderer {
    @BStaticMethod
    void setupDiskCache(File File0);
}
