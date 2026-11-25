# module: ParticleElementProperty

import sys

DelayTime: int = 0x130
DelayTimeRandom: int = 0x134
DepthOffset: int = 0x12C
DepthTest: int = 0x102
DepthWrite: int = 0x103
Distortion: int = 0x100
DynamicBatch: int = 0x144
FadeDistance: int = 0x128
LODGroup: int = 0x52
Local: int = 0x101
MaterialInfo: int = 0x58
MaxParticles: int = 0x50
OffsetScreen: int = 0x140
RandomSeed: int = 0x13C
RenderSortMode: int = 0x142
Rotation: int = 0x110
Scale: int = 0x104
SortMode: int = 0x53
Translation: int = 0x11C
WarmUpTime: int = 0x138

def GetParticleElementProperty(): ...
