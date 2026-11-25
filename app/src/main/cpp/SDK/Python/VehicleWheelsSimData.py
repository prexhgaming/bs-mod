# module: VehicleWheelsSimData

import sys

AntiRollBars: int = 0x18
HighForwardSpeedSubStepCount: int = 0x50
LowForwardSpeedSubStepCount: int = 0x4C
MinLongSlipDenominator: int = 0x54
SupsStickDatas: int = 0x30
ThresholdLongitudinalSpeed: int = 0x48
TireLoadFilter: int = 0x38
UseSweepQuery: int = 0x58

def GetVehicleWheelsSimData(): ...
