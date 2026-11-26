// IBPackageInstallerService.aidl
package zyn.bs.core.core.system.pm;

import zyn.bs.core.core.system.pm.BPackageSettings;
import zyn.bs.core.entity.pm.InstallOption;

// Declare any non-default types here with import statements

interface IBPackageInstallerService {
    int installPackageAsUser(in BPackageSettings ps, int userId);
    int uninstallPackageAsUser(in BPackageSettings ps, boolean removeApp, int userId);
    int clearPackage(in BPackageSettings ps, int userId);
    int updatePackage(in BPackageSettings ps);
}
