# module: VehicleDriveDesc

import sys

AutoBox: int = 0x94
Clutch: int = 0x108
Engine: int = 0x20
Gear: int = 0x50
Steer: int = 0x10C

def GetVehicleDriveDesc(): ...
