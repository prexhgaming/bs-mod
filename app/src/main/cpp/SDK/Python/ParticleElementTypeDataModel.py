# module: ParticleElementTypeDataModel

import sys

Animation: int = 0x80
DoubleSided: int = 0x9B
Effect: int = 0x98
FaceCamera: int = 0x9D
FixedRotation: int = 0x9E
HideTPos: int = 0xA4
Loop: int = 0xA2
ModelPath: int = 0x50
OriginalMaterial: int = 0x99
PlaySpeed: int = 0xA8
PostBlend: int = 0x9C
RemoveMotion: int = 0xA3
RepeatU: int = 0xA0
RepeatV: int = 0xA1
SkeletonPath: int = 0x68
Solid: int = 0x9A
YAxisLocked: int = 0x9F

def GetParticleElementTypeDataModel(): ...
