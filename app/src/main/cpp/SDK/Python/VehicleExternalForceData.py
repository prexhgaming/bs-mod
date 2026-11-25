# module: VehicleExternalForceData

import sys

Enabled: int = 0x19
InLocalFrame: int = 0x1A
Mode: int = 0x18
Torque: int = 0xC

def GetVehicleExternalForceData(): ...
