# module: LerpBlender

import sys

BlendTime: int = 0xB8
BlendType: int = 0xC4
DirectTransit: int = 0xC2
Escaped: int = 0xBC
Finished: int = 0xC0
InvalidLastInfo: int = 0xC1
LastDirection: int = 0xA4
LastFov: int = 0xB0
LastRoll: int = 0xB4
LastTargetPos: int = 0x98

def GetLerpBlender(): ...
