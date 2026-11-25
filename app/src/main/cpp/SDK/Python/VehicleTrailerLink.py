# module: VehicleTrailerLink

import sys

Ackermann: int = 0x28
AnchorOnChildF: int = 0xC
Angle0Limit: int = 0x18
Angle1Limit: int = 0x1C
Angle2Limit: int = 0x20
EnableBrake: int = 0x24
EnableSteer: int = 0x25

def GetVehicleTrailerLink(): ...
