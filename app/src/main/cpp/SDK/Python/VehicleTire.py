# module: VehicleTire

import sys

CamberStiffnessPerUnitGravity: int = 0x4C
FrictionVsSlip00: int = 0x50
FrictionVsSlip01: int = 0x54
FrictionVsSlip10: int = 0x58
FrictionVsSlip11: int = 0x5C
FrictionVsSlip20: int = 0x60
FrictionVsSlip21: int = 0x64
LatStiffX: int = 0x40
LatStiffY: int = 0x44
LongitudinalStiffnessPerUnitGravity: int = 0x48
TireType: int = 0x68

def GetVehicleTire(): ...
