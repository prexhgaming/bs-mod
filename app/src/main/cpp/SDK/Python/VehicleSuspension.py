# module: VehicleSuspension

import sys

CamberAtMaxCompression: int = 0x58
CamberAtMaxDroop: int = 0x5C
CamberAtRest: int = 0x54
MaxCompression: int = 0x48
MaxDroop: int = 0x4C
SpringDamperRate: int = 0x44
SpringStrength: int = 0x40
SprungMass: int = 0x50

def GetVehicleSuspension(): ...
