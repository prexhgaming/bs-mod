# module: VehicleWheelData

import sys

CenterOffset: int = 0x24
DampingRate: int = 0x10
MOI: int = 0xC
MaxBrakeTorque: int = 0x14
MaxHandBrakeTorque: int = 0x18
MaxSteer: int = 0x1C
ToeAngle: int = 0x20

def GetVehicleWheelData(): ...
