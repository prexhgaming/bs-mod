package black.com.android.internal.content;

import android.content.Intent;

import zyn.bs.core.reflection.annotation.BClassName;
import zyn.bs.core.reflection.annotation.BConstructor;

@BClassName("com.android.internal.content.ReferrerIntent")
public interface ReferrerIntent {
    @BConstructor
    Intent _new(Intent Intent0, String String1);
}
