# module: VehicleEulerAngleStabilizerData

import sys

ActiveMode: int = 0x21
AngleIndex: int = 0x20
DampingRatio: int = 0xC
Enabled: int = 0x22
HighActiveAngle: int = 0x18
LowActiveAngle: int = 0x14
MaxAngAcc: int = 0x1C
NaturalFrequency: int = 0x10

def GetVehicleEulerAngleStabilizerData(): ...
