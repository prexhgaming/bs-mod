# module: ParticleElementTypeDataBlade

import sys

DistanceSampleInterval: int = 0x50
FixOrigin: int = 0x68
MaxSampleCount: int = 0x5C
RepeatU: int = 0x69
RepeatV: int = 0x6A
RotationSampleInterval: int = 0x54
SizeSampleInterval: int = 0x58
Tension: int = 0x60
Thickness: int = 0x64

def GetParticleElementTypeDataBlade(): ...
