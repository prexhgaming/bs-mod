package zyn.bs.core.core.system.pm.installer;

import zyn.bs.core.core.env.BEnvironment;
import zyn.bs.core.core.system.pm.BPackageSettings;
import zyn.bs.core.entity.pm.InstallOption;
import zyn.bs.core.utils.FileUtils;


public class RemoveUserExecutor implements Executor {

    @Override
    public int exec(BPackageSettings ps, InstallOption option, int userId) {
        String packageName = ps.pkg.packageName;
        
        FileUtils.deleteDir(BEnvironment.getDataDir(packageName, userId));
        FileUtils.deleteDir(BEnvironment.getDeDataDir(packageName, userId));
        FileUtils.deleteDir(BEnvironment.getExternalDataDir(packageName, userId));
        return 0;
    }
}
