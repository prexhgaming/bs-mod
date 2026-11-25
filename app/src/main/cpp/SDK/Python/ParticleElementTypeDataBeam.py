# module: ParticleElementTypeDataBeam

import sys

AttackDistance: int = 0x9C
AxialVariation: int = 0x98
ColorControl: int = 0xE0
EnableControl: int = 0xC4
EnableTangents: int = 0xA4
FixedSubdivisions: int = 0x51
FixedTextureRepeat: int = 0x50
NoiseLevel: int = 0x64
NoiseScales: int = 0x68
NoiseWeights: int = 0x78
RadialByVelocity: int = 0x94
RadialVariation: int = 0x90
ReleaseDistance: int = 0xA0
SizeControl: int = 0xC8
SourceTangent: int = 0xA8
SpaceFrequency: int = 0x88
SubdivisionCount: int = 0x5C
SubdivisionPeriod: int = 0x60
TangentAdaptive: int = 0xA5
TargetTangent: int = 0xB0
TextureRepeatCount: int = 0x54
TextureRepeatPeriod: int = 0x58
TimeFrequency: int = 0x8C
UVPanningRate: int = 0xBC
UVPanningTailsExtra: int = 0xC0
UVScollingSpeed: int = 0xB8

def GetParticleElementTypeDataBeam(): ...
