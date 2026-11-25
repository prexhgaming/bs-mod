# module: VehicleDynamicState

import sys

DestVehicleForwardSpeed: int = 0x10
EngineSpeed: int = 0x18
EngineSpeedDiff: int = 0x1C
ForwardSpeedSmoothSpeed: int = 0x14
InputAccel: int = 0x20
InputBrake: int = 0x24
InputHandBrake: int = 0x28
InputSteerLeft: int = 0x2C
InputSteerRight: int = 0x30
VehicleForwardSpeed: int = 0xC
Wheels: int = 0x38

def GetVehicleDynamicState(): ...
