# module: CoupleFocusLocator

import sys

Direction: int = 0xB8
MajorTarget: int = 0xC8
MajorTargetPos: int = 0x88
MajorTargetPosOffset: int = 0xA0
MajorTargetTransform: int = 0xD8
MinorTarget: int = 0xD0
MinorTargetPos: int = 0x94
MinorTargetPosOffset: int = 0xAC
MinorTargetTransform: int = 0x108

def GetCoupleFocusLocator(): ...
