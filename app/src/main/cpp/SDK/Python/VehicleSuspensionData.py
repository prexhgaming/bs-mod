# module: VehicleSuspensionData

import sys

CamberAtMaxCompression: int = 0x18
CamberAtMaxDroop: int = 0x1C
CamberAtRest: int = 0x14
ForceApplyPointOffset: int = 0x2C
MaxDroop: int = 0xC
SprungMass: int = 0x10
TravelDirection: int = 0x20

def GetVehicleSuspensionData(): ...
