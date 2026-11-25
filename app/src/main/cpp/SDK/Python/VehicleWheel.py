# module: VehicleWheel

import sys

DampingRate: int = 0x50
MOI: int = 0x4C
Mass: int = 0x48
MaxBrakeTorque: int = 0x54
MaxHandBrakeTorque: int = 0x58
MaxSteer: int = 0x5C
Radius: int = 0x40
ToeAngle: int = 0x60
Width: int = 0x44

def GetVehicleWheel(): ...
