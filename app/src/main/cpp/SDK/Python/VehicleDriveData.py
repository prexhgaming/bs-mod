# module: VehicleDriveData

import sys

Ackermann: int = 0x110
AutoBox: int = 0x90
Clutch: int = 0x104
Differential: int = 0x30
Gear: int = 0x4C

def GetVehicleDriveData(): ...
