# module: ModifierBlender

import sys

BlendTime: int = 0x98
Direction: int = 0xB0
EnableModifier: int = 0xA3
Finished: int = 0xA0
Fov: int = 0xBC
InvalidLastInfo: int = 0xC0
TargetPos: int = 0xA4

def GetModifierBlender(): ...
