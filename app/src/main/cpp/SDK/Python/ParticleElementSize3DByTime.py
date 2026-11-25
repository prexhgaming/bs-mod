# module: ParticleElementSize3DByTime

import sys

CurveGuid: int = 0xA0
Loop: int = 0x54
OverLife: int = 0x55
Peroid: int = 0x50
SizeCurveX: int = 0x58
SizeCurveY: int = 0x70
SizeCurveZ: int = 0x88

def GetParticleElementSize3DByTime(): ...
