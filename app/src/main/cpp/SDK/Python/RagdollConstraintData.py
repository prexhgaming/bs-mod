# module: RagdollConstraintData

import sys

LinearLimitDamping: int = 0xC4
LinearLimitExtend: int = 0xBC
LinearLimitStiffness: int = 0xC0
SwingLimitDamping: int = 0xD4
SwingLimitStiffness: int = 0xD0
SwingLimitY: int = 0xC8
SwingLimitZ: int = 0xCC
SwingYMotion: int = 0xB9
SwingZMotion: int = 0xBA
TwistLimitDamping: int = 0xE4
TwistLimitHigh: int = 0xDC
TwistLimitLow: int = 0xD8
TwistLimitStiffness: int = 0xE0
TwistMotion: int = 0xB8
XAxisMotion: int = 0xB5
YAxisMotion: int = 0xB6
ZAxisMotion: int = 0xB7

def GetRagdollConstraintData(): ...
