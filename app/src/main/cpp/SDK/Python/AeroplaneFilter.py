# module: AeroplaneFilter

import sys

ApplyMotion: int = 0x80
DebugOutput: int = 0x81
DriveRB: int = 0x118
InterpThreshold: int = 0x88
TimeErrorTH: int = 0x84
World: int = 0xE8

def GetAeroplaneFilter(): ...
