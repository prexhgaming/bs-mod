# module: VehiclePadSmoothData

import sys

BrakeFallRate: int = 0xC
HandBrakeFallRate: int = 0x14
HandBrakeRiseRate: int = 0x10
SteerLeftFallRate: int = 0x1C
SteerLeftRiseRate: int = 0x18
SteerRightFallRate: int = 0x24
SteerRightRiseRate: int = 0x20

def GetVehiclePadSmoothData(): ...
