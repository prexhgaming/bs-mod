# module: SingleFocusLocator

import sys

Direction: int = 0xA0
Target: int = 0xB0
TargetPos: int = 0x88
TargetPosOffset: int = 0x94
TargetTransform: int = 0xB8

def GetSingleFocusLocator(): ...
