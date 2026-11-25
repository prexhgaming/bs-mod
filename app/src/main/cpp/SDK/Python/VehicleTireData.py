# module: VehicleTireData

import sys

CamberStiffnessPerUnitGravity: int = 0xC
FrictionVsSlip: int = 0x14
TireForceApplyCMOffsets: int = 0x2C
Type: int = 0x10

def GetVehicleTireData(): ...
