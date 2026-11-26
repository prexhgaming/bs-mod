package zyn.bs.core.fake.hook;

import android.util.Log;

import java.util.HashMap;
import java.util.Map;

import zyn.bs.core.BlackBoxCore;
import zyn.bs.core.fake.delegate.AppInstrumentation;

import zyn.bs.core.fake.service.HCallbackProxy;
import zyn.bs.core.fake.service.IAccessibilityManagerProxy;
import zyn.bs.core.fake.service.IAccountManagerProxy;
import zyn.bs.core.fake.service.IActivityClientProxy;
import zyn.bs.core.fake.service.IActivityManagerProxy;
import zyn.bs.core.fake.service.IActivityTaskManagerProxy;
import zyn.bs.core.fake.service.IAlarmManagerProxy;
import zyn.bs.core.fake.service.IAppOpsManagerProxy;
import zyn.bs.core.fake.service.IAppWidgetManagerProxy;
import zyn.bs.core.fake.service.IAttributionSourceProxy;
import zyn.bs.core.fake.service.IAutofillManagerProxy;
import zyn.bs.core.fake.service.ISensitiveContentProtectionManagerProxy;
import zyn.bs.core.fake.service.ISettingsSystemProxy;
import zyn.bs.core.fake.service.IConnectivityManagerProxy;
import zyn.bs.core.fake.service.ISystemSensorManagerProxy;
import zyn.bs.core.fake.service.IContentProviderProxy;
import zyn.bs.core.fake.service.IXiaomiAttributionSourceProxy;
import zyn.bs.core.fake.service.IXiaomiSettingsProxy;
import zyn.bs.core.fake.service.IXiaomiMiuiServicesProxy;
import zyn.bs.core.fake.service.IDnsResolverProxy;
import zyn.bs.core.fake.service.IContextHubServiceProxy;
import zyn.bs.core.fake.service.IDeviceIdentifiersPolicyProxy;
import zyn.bs.core.fake.service.IDevicePolicyManagerProxy;
import zyn.bs.core.fake.service.IDisplayManagerProxy;
import zyn.bs.core.fake.service.IFingerprintManagerProxy;
import zyn.bs.core.fake.service.IGraphicsStatsProxy;
import zyn.bs.core.fake.service.IJobServiceProxy;
import zyn.bs.core.fake.service.ILauncherAppsProxy;
import zyn.bs.core.fake.service.ILocationManagerProxy;
import zyn.bs.core.fake.service.IMediaRouterServiceProxy;
import zyn.bs.core.fake.service.IMediaSessionManagerProxy;
import zyn.bs.core.fake.service.IAudioServiceProxy;
import zyn.bs.core.fake.service.ISensorPrivacyManagerProxy;
import zyn.bs.core.fake.service.ContentResolverProxy;
import zyn.bs.core.fake.service.IMiuiSecurityManagerProxy;
import zyn.bs.core.fake.service.SystemLibraryProxy;
import zyn.bs.core.fake.service.ReLinkerProxy;
import zyn.bs.core.fake.service.MediaRecorderProxy;
import zyn.bs.core.fake.service.NetworkPermissionCompat;
import zyn.bs.core.fake.service.AudioRecordProxy;
import zyn.bs.core.fake.service.MediaRecorderClassProxy;
import zyn.bs.core.fake.service.SQLiteDatabaseProxy;
import zyn.bs.core.fake.service.ClassLoaderProxy;
import zyn.bs.core.fake.service.FileSystemProxy;
import zyn.bs.core.fake.service.GmsProxy;
import zyn.bs.core.fake.service.LevelDbProxy;
import zyn.bs.core.fake.service.DeviceIdProxy;
import zyn.bs.core.fake.service.GoogleAccountManagerProxy;
import zyn.bs.core.fake.service.AuthenticationProxy;
import zyn.bs.core.fake.service.AndroidIdProxy;
import zyn.bs.core.fake.service.AudioPermissionProxy;
import zyn.bs.core.fake.service.NetworkPermissionCompat;

import zyn.bs.core.fake.service.INetworkManagementServiceProxy;
import zyn.bs.core.fake.service.INotificationManagerProxy;
import zyn.bs.core.fake.service.IPackageManagerProxy;
import zyn.bs.core.fake.service.IPermissionManagerProxy;
import zyn.bs.core.fake.service.IPersistentDataBlockServiceProxy;
import zyn.bs.core.fake.service.IPhoneSubInfoProxy;
import zyn.bs.core.fake.service.IPowerManagerProxy;
import zyn.bs.core.fake.service.ApkAssetsProxy;
import zyn.bs.core.fake.service.ResourcesManagerProxy;
import zyn.bs.core.fake.service.IShortcutManagerProxy;
import zyn.bs.core.fake.service.IStorageManagerProxy;
import zyn.bs.core.fake.service.IStorageStatsManagerProxy;
import zyn.bs.core.fake.service.ISystemUpdateProxy;
import zyn.bs.core.fake.service.ITelephonyManagerProxy;
import zyn.bs.core.fake.service.ITelephonyRegistryProxy;
import zyn.bs.core.fake.service.IUserManagerProxy;
import zyn.bs.core.fake.service.IVibratorServiceProxy;
import zyn.bs.core.fake.service.IVpnManagerProxy;
import zyn.bs.core.fake.service.IWifiManagerProxy;
import zyn.bs.core.fake.service.IWifiScannerProxy;
import zyn.bs.core.fake.service.IWindowManagerProxy;
import zyn.bs.core.fake.service.context.ContentServiceStub;
import zyn.bs.core.fake.service.context.RestrictionsManagerStub;
import zyn.bs.core.fake.service.libcore.OsStub;
import zyn.bs.core.utils.Slog;
import zyn.bs.core.utils.compat.BuildCompat;
import zyn.bs.core.fake.service.ISettingsProviderProxy;
import zyn.bs.core.fake.service.FeatureFlagUtilsProxy;
import zyn.bs.core.fake.service.WorkManagerProxy;



public class HookManager {
    public static final String TAG = "HookManager";

    private static final HookManager sHookManager = new HookManager();

    private final Map<Class<?>, IInjectHook> mInjectors = new HashMap<>();

    public static HookManager get() {
        return sHookManager;
    }

    public void init() {
        if (BlackBoxCore.get().isBlackProcess() || BlackBoxCore.get().isServerProcess()) {
            addInjector(new IDisplayManagerProxy());
            addInjector(new OsStub());
            addInjector(new IActivityManagerProxy());
            addInjector(new IPackageManagerProxy());
            addInjector(new ITelephonyManagerProxy());
            addInjector(new HCallbackProxy());
            addInjector(new IAppOpsManagerProxy());
            addInjector(new INotificationManagerProxy());
            addInjector(new IAlarmManagerProxy());
            addInjector(new IAppWidgetManagerProxy());
            addInjector(new ContentServiceStub());
            addInjector(new IWindowManagerProxy());
            addInjector(new IUserManagerProxy());
            addInjector(new RestrictionsManagerStub());
            addInjector(new IMediaSessionManagerProxy());
            addInjector(new IAudioServiceProxy());
            addInjector(new ISensorPrivacyManagerProxy());
            addInjector(new ContentResolverProxy());
            addInjector(new SystemLibraryProxy());
            addInjector(new ReLinkerProxy());
            addInjector(new WorkManagerProxy());
            addInjector(new MediaRecorderProxy());
            addInjector(new AudioRecordProxy());
            addInjector(new IMiuiSecurityManagerProxy());
            addInjector(new ISettingsProviderProxy());
            addInjector(new FeatureFlagUtilsProxy());
            addInjector(new MediaRecorderClassProxy());
            addInjector(new SQLiteDatabaseProxy());
            addInjector(new ClassLoaderProxy());
            addInjector(new FileSystemProxy());
            addInjector(new GmsProxy());
            addInjector(new LevelDbProxy());
            addInjector(new DeviceIdProxy());
            addInjector(new GoogleAccountManagerProxy());
            addInjector(new AuthenticationProxy());
            addInjector(new AndroidIdProxy());
            addInjector(new AudioPermissionProxy());
            addInjector(new ILocationManagerProxy());
            addInjector(new IStorageManagerProxy());
            addInjector(new ILauncherAppsProxy());
            addInjector(new IJobServiceProxy());
            addInjector(new IAccessibilityManagerProxy());
            addInjector(new ITelephonyRegistryProxy());
            addInjector(new IDevicePolicyManagerProxy());
            addInjector(new IAccountManagerProxy());
            addInjector(new NetworkPermissionCompat());
            addInjector(new IConnectivityManagerProxy());
            addInjector(new IDnsResolverProxy());
                    addInjector(new IAttributionSourceProxy());
        addInjector(new IContentProviderProxy());
        addInjector(new ISettingsSystemProxy());
        addInjector(new ISystemSensorManagerProxy());
        
        
        addInjector(new IXiaomiAttributionSourceProxy());
        addInjector(new IXiaomiSettingsProxy());
        addInjector(new IXiaomiMiuiServicesProxy());
            addInjector(new IPhoneSubInfoProxy());
            addInjector(new IMediaRouterServiceProxy());
            addInjector(new IPowerManagerProxy());
            addInjector(new IContextHubServiceProxy());
            
            addInjector(new IVibratorServiceProxy());
            addInjector(new IPersistentDataBlockServiceProxy());
            addInjector(AppInstrumentation.get());
            
            addInjector(new IWifiManagerProxy());
            addInjector(new IWifiScannerProxy());
            addInjector(new ApkAssetsProxy());
            addInjector(new ResourcesManagerProxy());
            
            if (BuildCompat.isS()) {
                addInjector(new IActivityClientProxy(null));
                addInjector(new IVpnManagerProxy());
            }
            
            if (BuildCompat.isS()) {
                addInjector(new ISensitiveContentProtectionManagerProxy());
            }
            
            if (BuildCompat.isR()) {
                addInjector(new IPermissionManagerProxy());
            }
            
            if (BuildCompat.isQ()) {
                addInjector(new IActivityTaskManagerProxy());
            }
            
            if (BuildCompat.isPie()) {
                addInjector(new ISystemUpdateProxy());
            }
            
            if (BuildCompat.isOreo()) {
                addInjector(new IAutofillManagerProxy());
                addInjector(new IDeviceIdentifiersPolicyProxy());
                addInjector(new IStorageStatsManagerProxy());
            }
            
            if (BuildCompat.isN_MR1()) {
                addInjector(new IShortcutManagerProxy());
            }
            
            if (BuildCompat.isN()) {
                addInjector(new INetworkManagementServiceProxy());
            }
            
            if (BuildCompat.isM()) {
                addInjector(new IFingerprintManagerProxy());
                addInjector(new IGraphicsStatsProxy());
            }
            
            if (BuildCompat.isL()) {
                addInjector(new IJobServiceProxy());
            }
        }
        injectAll();
    }

    public void checkEnv(Class<?> clazz) {
        IInjectHook iInjectHook = mInjectors.get(clazz);
        if (iInjectHook != null && iInjectHook.isBadEnv()) {
            Log.d(TAG, "checkEnv: " + clazz.getSimpleName() + " is bad env");
            iInjectHook.injectHook();
        }
    }

    public void checkAll() {
        for (Class<?> aClass : mInjectors.keySet()) {
            IInjectHook iInjectHook = mInjectors.get(aClass);
            if (iInjectHook != null && iInjectHook.isBadEnv()) {
                Log.d(TAG, "checkEnv: " + aClass.getSimpleName() + " is bad env");
                iInjectHook.injectHook();
            }
        }
    }

    void addInjector(IInjectHook injectHook) {
        mInjectors.put(injectHook.getClass(), injectHook);
    }

    void injectAll() {
        for (IInjectHook value : mInjectors.values()) {
            try {
                Slog.d(TAG, "hook: " + value);
                value.injectHook();
            } catch (Exception e) {
                Slog.d(TAG, "hook error: " + value);
                
                handleHookError(value, e);
            }
        }
    }

    
    private void handleHookError(IInjectHook hook, Exception e) {
        String hookName = hook.getClass().getSimpleName();
        
        
        Slog.e(TAG, "Hook failed: " + hookName + " - " + e.getMessage(), e);
        
        
        if (hookName.contains("ActivityManager") || 
            hookName.contains("PackageManager") ||
            hookName.contains("WebView") ||
            hookName.contains("ContentProvider")) {
            
            Slog.w(TAG, "Critical hook failed: " + hookName + ", attempting recovery");
            
            try {
                
                if (hook.isBadEnv()) {
                    Slog.d(TAG, "Attempting to recover hook: " + hookName);
                    hook.injectHook();
                }
            } catch (Exception recoveryException) {
                Slog.e(TAG, "Hook recovery failed: " + hookName, recoveryException);
            }
        }
    }

    
    public boolean areCriticalHooksInstalled() {
        String[] criticalHooks = {
            "IActivityManagerProxy",
            "IPackageManagerProxy",
            "IContentProviderProxy"
        };
        
        for (String hookName : criticalHooks) {
            boolean found = false;
            for (Class<?> hookClass : mInjectors.keySet()) {
                if (hookClass.getSimpleName().equals(hookName)) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                Slog.w(TAG, "Critical hook missing: " + hookName);
                return false;
            }
        }
        
        Slog.d(TAG, "All critical hooks are installed");
        return true;
    }

    
    public void reinitializeHooks() {
        Slog.d(TAG, "Reinitializing all hooks");
        
        
        mInjectors.clear();
        
        
        init();
        
        Slog.d(TAG, "Hook reinitialization completed");
    }
}
