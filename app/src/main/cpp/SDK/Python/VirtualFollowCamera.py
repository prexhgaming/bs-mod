# module: VirtualFollowCamera

import sys

AutoUpdate: int = 0xE8
FollowRotation: int = 0xF8
FollowTarget: int = 0xE0
GlobalOffset: int = 0xFC
LocalOffset: int = 0x12C

def GetVirtualFollowCamera(): ...
