# module: VehicleThrustVectorData

import sys

Accel: int = 0x14
BackwardSpeed: int = 0x10
Enabled: int = 0x3C
ForwardSpeed: int = 0xC
SideAcc: int = 0x18
SideAccel: int = 0x20
SideSpeed: int = 0x1C
Steer: int = 0x30
SteerDownAcc: int = 0x28
SteerSpeed: int = 0x2C
SteerUpAcc: int = 0x24
WindDragMinAcc: int = 0x38
WindDragRatio: int = 0x34

def GetVehicleThrustVectorData(): ...
