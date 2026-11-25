# module: VehicleDynamicStateWrapper

import sys

CurrentGear: int = 0x44
DestVehicleForwardSpeed: int = 0x50
EngineSpeed: int = 0x58
EngineSpeedDiff: int = 0x5C
ForwardSpeedSmoothSpeed: int = 0x54
GarDownPressed: int = 0x42
GearUpPressed: int = 0x41
InputAccel: int = 0x60
InputBrake: int = 0x64
InputHandBrake: int = 0x68
InputSteerLeft: int = 0x6C
InputSteerRight: int = 0x70
TargetGear: int = 0x48
UseAutoGears: int = 0x40
VehicleForwardSpeed: int = 0x4C
Wheels: int = 0x78

def GetVehicleDynamicStateWrapper(): ...
