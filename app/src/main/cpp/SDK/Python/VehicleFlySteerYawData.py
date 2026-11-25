# module: VehicleFlySteerYawData

import sys

Enabled: int = 0x14
SideDampRatio: int = 0x10
Steer: int = 0xC

def GetVehicleFlySteerYawData(): ...
