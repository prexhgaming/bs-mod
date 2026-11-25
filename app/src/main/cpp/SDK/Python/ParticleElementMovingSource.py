# module: ParticleElementMovingSource

import sys

AccelerationControl: int = 0x88
Direction: int = 0x68
Distance: int = 0x74
EmissionRate: int = 0x50
EndRadius: int = 0x7C
Loop: int = 0x58
Period: int = 0x54
StartOrbitPhase: int = 0x80
StartPoint: int = 0x5C
StartRadius: int = 0x78
Turns: int = 0x84

def GetParticleElementMovingSource(): ...
