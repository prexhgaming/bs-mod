# module: VehicleSuspensionDesc

import sys

CamberAtMaxCompression: int = 0x18
CamberAtMaxDroop: int = 0x1C
CamberAtRest: int = 0x14
MaxDroop: int = 0xC
SprungMass: int = 0x10

def GetVehicleSuspensionDesc(): ...
