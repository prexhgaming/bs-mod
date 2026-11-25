# module: CommonVehicleController

import sys

EnableAutoReverse: int = 0x49
GearDown: int = 0x46
GearUp: int = 0x45
KeyPressedAccel: int = 0x40
KeyPressedBrake: int = 0x41
KeyPressedHandbrake: int = 0x42
KeyPressedSteerLeft: int = 0x43
KeyPressedSteerRight: int = 0x44
PadAccel: int = 0x4C
PadBrake: int = 0x50
PadHandbrake: int = 0x54
PadSteer: int = 0x58
UseAutoGears: int = 0x48
UseKey: int = 0x47

def GetCommonVehicleController(): ...
