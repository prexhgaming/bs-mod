# module: VehicleEngineData

import sys

DampingRateFullThrottle: int = 0xC
DampingRateZeroThrottleClutchDisengaged: int = 0x14
DampingRateZeroThrottleClutchEngaged: int = 0x10
TorqueCurve: int = 0x18

def GetVehicleEngineData(): ...
