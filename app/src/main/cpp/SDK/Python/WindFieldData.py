# module: WindFieldData

import sys

AdvectionBlendParam: int = 0x80
AmbientNoiseTexId: int = 0x40
AmbientTexToWorldMap: int = 0x24
AmbientUVSize: int = 0x30
AmbientWind: int = 0x18
AmbientWindDisturbance: int = 0x50
DeltaTime: int = 0x78
DiffusionRate: int = 0x7C
EnableAdvection: int = 0x8C
EnableDiffusion: int = 0x8D
EnableNoiseAmbient: int = 0x8F
EnableProjection: int = 0x8E
FieldSize: int = 0x68
GridLength: int = 0x74
MinPos: int = 0x5C
NBDiffusionIter: int = 0x84
NBProjectIter: int = 0x88
NoiseSampleLevel: int = 0x54
NoiseUVResetTime: int = 0x58
constantWind: int = 0xC

def GetWindFieldData(): ...
