package zyn.bs.core.fake.service;

import android.content.Context;

import java.lang.reflect.Method;

import black.android.media.BRIMediaRouterServiceStub;
import black.android.os.BRServiceManager;
import zyn.bs.core.fake.hook.BinderInvocationStub;
import zyn.bs.core.fake.hook.MethodHook;
import zyn.bs.core.fake.hook.ProxyMethod;
import zyn.bs.core.utils.MethodParameterUtils;


public class IMediaRouterServiceProxy extends BinderInvocationStub {

    public IMediaRouterServiceProxy() {
        super(BRServiceManager.get().getService(Context.MEDIA_ROUTER_SERVICE));
    }

    @Override
    protected Object getWho() {
        return BRIMediaRouterServiceStub.get().asInterface(BRServiceManager.get().getService(Context.MEDIA_ROUTER_SERVICE));
    }

    @Override
    protected void inject(Object baseInvocation, Object proxyInvocation) {
        replaceSystemService(Context.MEDIA_ROUTER_SERVICE);
    }

    @Override
    public boolean isBadEnv() {
        return false;
    }

    @ProxyMethod("registerClientAsUser")
    public static class registerClientAsUser extends MethodHook {

        @Override
        protected Object hook(Object who, Method method, Object[] args) throws Throwable {
            MethodParameterUtils.replaceFirstAppPkg(args);
            return method.invoke(who, args);
        }
    }

    @ProxyMethod("registerRouter2")
    public static class registerRouter2 extends MethodHook {

        @Override
        protected Object hook(Object who, Method method, Object[] args) throws Throwable {
            MethodParameterUtils.replaceFirstAppPkg(args);
            return method.invoke(who, args);
        }
    }
}
