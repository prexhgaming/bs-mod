# module: ParticleElementMotion

import sys

Animation: int = 0x68
AutoClear: int = 0xBB
AutoReverse: int = 0xB9
AutoStop: int = 0xBA
BoneName: int = 0x80
CurveSpeed: int = 0xA0
LocalMotion: int = 0xBC
Loop: int = 0xB8
SkeletonPath: int = 0x50
Speed: int = 0x98

def GetParticleElementMotion(): ...
