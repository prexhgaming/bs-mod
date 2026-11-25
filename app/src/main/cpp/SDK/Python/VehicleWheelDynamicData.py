# module: VehicleWheelDynamicData

import sys

Jounce: int = 0x14
TireLowForwardSpeedTimer: int = 0xC
TireLowSideSpeedTimer: int = 0x10

def GetVehicleWheelDynamicData(): ...
