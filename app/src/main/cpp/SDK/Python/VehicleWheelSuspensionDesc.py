# module: VehicleWheelSuspensionDesc

import sys

AntiRollBars: int = 0xA8
SuspForceAppCMOffsets: int = 0x78
SuspTravelDirections: int = 0x48
Suspensions: int = 0x30
TireForceAppCMOffsets: int = 0x90
TireLoadFilter: int = 0xC0
Tires: int = 0x18
WheelCentreCMOffsets: int = 0x60

def GetVehicleWheelSuspensionDesc(): ...
