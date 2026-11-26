package black.java.lang;

import java.util.List;

import zyn.bs.core.reflection.annotation.BClassName;
import zyn.bs.core.reflection.annotation.BField;

@BClassName("java.lang.ThreadGroup")
public interface ThreadGroup {
    @BField
    List<java.lang.ThreadGroup> groups();

    @BField
    java.lang.ThreadGroup parent();
}
