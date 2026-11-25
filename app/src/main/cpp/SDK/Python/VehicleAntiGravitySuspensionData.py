# module: VehicleAntiGravitySuspensionData

import sys

MaxCompression: int = 0x30
MaxDrop: int = 0x2C
RestPos: int = 0x18
SpringDampingRatio: int = 0x28
SpringPeriod: int = 0x24
TravelDirection: int = 0xC

def GetVehicleAntiGravitySuspensionData(): ...
