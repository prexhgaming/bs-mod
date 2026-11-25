# module: VehicleStateFilter

import sys

ApplyMotion: int = 0x80
DTimeFactorDownT: int = 0x88
DTimeFactorKeepT: int = 0x8C
DTimeFactorStep: int = 0x90
DTimeFactorUpT: int = 0x84
DebugOutput: int = 0x81
DriveRB: int = 0x130
TimeErrorTH: int = 0x94
UseTaskGraph: int = 0xF8
World: int = 0x100

def GetVehicleStateFilter(): ...
