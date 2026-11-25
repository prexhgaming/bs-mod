# module: VehicleSteadyParamsWrapper

import sys

MaxRollVel: int = 0x6D46046000
RollSteadyActiveThreshold: int = 0x6D46046000
TargetRollAngleVsSteerRatio: int = 0x6D46046000

def GetVehicleSteadyParamsWrapper(): ...
