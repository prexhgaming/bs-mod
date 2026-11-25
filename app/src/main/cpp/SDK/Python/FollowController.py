# module: FollowController

import sys

FollowRotation: int = 0x98
GlobalOffset: int = 0x9C
LocalOffset: int = 0xCC
Pitch: int = 0x108
Position: int = 0xFC
Roll: int = 0x110
Target: int = 0x90
Yaw: int = 0x10C

def GetFollowController(): ...
