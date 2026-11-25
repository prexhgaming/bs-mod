# module: VehicleChassis

import sys

AngularDamping: int = 0x60
CMOffset: int = 0x50
LinearDamping: int = 0x5C
MOI: int = 0x40
Mass: int = 0x4C
SleepThreshold: int = 0x64

def GetVehicleChassis(): ...
