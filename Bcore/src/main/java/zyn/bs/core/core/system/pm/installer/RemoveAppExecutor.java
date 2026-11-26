package zyn.bs.core.core.system.pm.installer;

import zyn.bs.core.core.env.BEnvironment;
import zyn.bs.core.core.system.pm.BPackageSettings;
import zyn.bs.core.entity.pm.InstallOption;
import zyn.bs.core.utils.FileUtils;


public class RemoveAppExecutor implements Executor {
    @Override
    public int exec(BPackageSettings ps, InstallOption option, int userId) {
        FileUtils.deleteDir(BEnvironment.getAppDir(ps.pkg.packageName));
        return 0;
    }
}
