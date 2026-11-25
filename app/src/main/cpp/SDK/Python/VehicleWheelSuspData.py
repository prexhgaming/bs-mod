# module: VehicleWheelSuspData

import sys

Enabled: int = 0xA4
MapShapeId: int = 0xA0
TireData: int = 0x68
WheelData: int = 0x38

def GetVehicleWheelSuspData(): ...
