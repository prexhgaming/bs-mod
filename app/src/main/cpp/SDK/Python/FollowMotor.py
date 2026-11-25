# module: FollowMotor

import sys

Direction: int = 0x128
LastTarget: int = 0x168
Target: int = 0x170
TargetPos: int = 0x110
TargetPosOffset: int = 0x11C
TargetTransform: int = 0x134

def GetFollowMotor(): ...
