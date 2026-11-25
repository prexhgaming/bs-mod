# module: VehicleDrive

import sys

AutoBox: int = 0x58
Clutch: int = 0x60
Differential: int = 0x40
DriveEngine: int = 0x48
Gear: int = 0x50
Steer: int = 0x68

def GetVehicleDrive(): ...
