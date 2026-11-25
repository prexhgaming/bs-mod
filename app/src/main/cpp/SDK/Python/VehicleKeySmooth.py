# module: VehicleKeySmooth

import sys

AccelFallRate: int = 0x44
AccelRiseRate: int = 0x40
BrakeFallRate: int = 0x4C
BrakeRiseRate: int = 0x48
HandBrakeFallRate: int = 0x54
HandBrakeRiseRate: int = 0x50
SteerLeftFallRate: int = 0x5C
SteerLeftRiseRate: int = 0x58
SteerRightFallRate: int = 0x64
SteerRightRiseRate: int = 0x60

def GetVehicleKeySmooth(): ...
