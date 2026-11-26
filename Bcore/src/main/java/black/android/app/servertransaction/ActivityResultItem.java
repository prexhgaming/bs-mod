package black.android.app.servertransaction;

import java.util.List;

import zyn.bs.core.reflection.annotation.BClassName;
import zyn.bs.core.reflection.annotation.BField;

@BClassName("android.app.servertransaction.ActivityResultItem")
public interface ActivityResultItem {
    @BField
    List mResultInfoList();
}
