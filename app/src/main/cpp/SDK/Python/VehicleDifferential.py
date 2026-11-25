# module: VehicleDifferential

import sys

CentreBias: int = 0x4C
DiffType: int = 0x58
FrontBias: int = 0x50
FrontLeftRightSplit: int = 0x44
FrontRearSplit: int = 0x40
RearBias: int = 0x54
RearLeftRightSplit: int = 0x48

def GetVehicleDifferential(): ...
