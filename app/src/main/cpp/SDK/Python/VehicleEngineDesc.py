# module: VehicleEngineDesc

import sys

DampingRateFullThrottle: int = 0xC
DampingRateZeroThrottleClutchDisengaged: int = 0x14
DampingRateZeroThrottleClutchEngaged: int = 0x10

def GetVehicleEngineDesc(): ...
