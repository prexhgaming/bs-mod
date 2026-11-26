package zyn.bs.core.fake.service;

import java.lang.reflect.Method;

import black.android.os.BRServiceManager;
import black.android.view.BRIGraphicsStatsStub;
import zyn.bs.core.fake.hook.BinderInvocationStub;
import zyn.bs.core.fake.hook.MethodHook;
import zyn.bs.core.fake.hook.ProxyMethod;
import zyn.bs.core.utils.MethodParameterUtils;


public class IGraphicsStatsProxy extends BinderInvocationStub {

    public IGraphicsStatsProxy() {
        super(BRServiceManager.get().getService("graphicsstats"));
    }

    @Override
    protected Object getWho() {
        return BRIGraphicsStatsStub.get().asInterface(BRServiceManager.get().getService("graphicsstats"));
    }

    @Override
    protected void inject(Object baseInvocation, Object proxyInvocation) {
        replaceSystemService("graphicsstats");
    }

    @Override
    public boolean isBadEnv() {
        return false;
    }

    @ProxyMethod("requestBufferForProcess")
    public static class RequestBufferForProcess extends MethodHook {
        @Override
        protected Object hook(Object who, Method method, Object[] args) throws Throwable {
            MethodParameterUtils.replaceFirstAppPkg(args);
            return method.invoke(who, args);
        }
    }
}
