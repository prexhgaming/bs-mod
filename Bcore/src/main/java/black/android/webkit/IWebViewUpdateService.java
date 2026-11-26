package black.android.webkit;


import zyn.bs.core.reflection.annotation.BClassName;
import zyn.bs.core.reflection.annotation.BMethod;

@BClassName("android.webkit.IWebViewUpdateService")
public interface IWebViewUpdateService {
    @BMethod
    String getCurrentWebViewPackageName();

    @BMethod
    Object waitForAndGetProvider();
}
