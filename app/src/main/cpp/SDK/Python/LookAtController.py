# module: LookAtController

import sys

Damping: int = 0xD8
DeadZone: int = 0xC8
Offset: int = 0xBC
Pitch: int = 0xE4
Position: int = 0xB0
SoftZone: int = 0xD0
Target: int = 0x90
TargetBone: int = 0x98
Yaw: int = 0xE8

def GetLookAtController(): ...
