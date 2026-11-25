# module: VehicleWheelSuspension

import sys

AntiRollBars: int = 0xE8
SuspForceAppCMOffsets: int = 0xB8
SuspTravelDirections: int = 0x88
Suspensions: int = 0x70
TireForceAppCMOffsets: int = 0xD0
TireLoadFilter: int = 0x100
Tires: int = 0x58
WheelCentreCMOffsets: int = 0xA0
WheelEnableFlags: int = 0x108
Wheels: int = 0x40

def GetVehicleWheelSuspension(): ...
