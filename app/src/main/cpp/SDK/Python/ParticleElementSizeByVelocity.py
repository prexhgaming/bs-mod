# module: ParticleElementSizeByVelocity

import sys

CoupledMode: int = 0x94
CurveGuid: int = 0x68
SizeCurve: int = 0x50
StretchScale: int = 0x88
VelocityMax: int = 0x84
VelocityMin: int = 0x80

def GetParticleElementSizeByVelocity(): ...
