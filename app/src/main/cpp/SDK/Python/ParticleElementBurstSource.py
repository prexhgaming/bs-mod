# module: ParticleElementBurstSource

import sys

CurveGuid: int = 0x70
EmissionBursts: int = 0x88
EmissionCurve: int = 0x58
EmissionDuration: int = 0x54
EmissionRate: int = 0x50
LoopPeriod: int = 0xA0

def GetParticleElementBurstSource(): ...
