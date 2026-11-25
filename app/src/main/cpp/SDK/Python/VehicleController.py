# module: VehicleController

import sys

EnableAutoReverse: int = 0x19
PadBrake: int = 0xC
PadHandbrake: int = 0x10
PadSteer: int = 0x14
UseKey: int = 0x18

def GetVehicleController(): ...
