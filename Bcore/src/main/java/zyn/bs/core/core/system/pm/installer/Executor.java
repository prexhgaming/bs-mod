package zyn.bs.core.core.system.pm.installer;

import zyn.bs.core.core.system.pm.BPackageSettings;
import zyn.bs.core.entity.pm.InstallOption;


public interface Executor {
    public static final String TAG = "InstallExecutor";

    int exec(BPackageSettings ps, InstallOption option, int userId);
}
