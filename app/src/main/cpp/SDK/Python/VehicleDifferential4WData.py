# module: VehicleDifferential4WData

import sys

CentreBias: int = 0xC
FrontBias: int = 0x10
RearBias: int = 0x14
Type: int = 0x18

def GetVehicleDifferential4WData(): ...
