# module: VehicleChassisDesc

import sys

AngularDamping: int = 0x20
CMOffset: int = 0x10
LinearDamping: int = 0x1C
Mass: int = 0xC
SleepThreshold: int = 0x24

def GetVehicleChassisDesc(): ...
