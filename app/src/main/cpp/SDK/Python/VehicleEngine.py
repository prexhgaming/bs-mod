# module: VehicleEngine

import sys

DampingRateFullThrottle: int = 0x4C
DampingRateZeroThrottleClutchDisengaged: int = 0x54
DampingRateZeroThrottleClutchEngaged: int = 0x50
MOI: int = 0x40
MaxOmega: int = 0x48
PeakTorque: int = 0x44
TorqueCurveEngineSpeed: int = 0x58
TorqueCurveEngineTorque: int = 0x70

def GetVehicleEngine(): ...
