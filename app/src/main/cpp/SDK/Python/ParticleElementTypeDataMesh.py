# module: ParticleElementTypeDataMesh

import sys

CenterOffset: int = 0x68
DoubleSided: int = 0x76
Effect: int = 0x74
FaceCamera: int = 0x78
FixedRotation: int = 0x79
MeshPath: int = 0x50
PostBlend: int = 0x77
RepeatU: int = 0x7B
RepeatV: int = 0x7C
Solid: int = 0x75
YAxisLocked: int = 0x7A

def GetParticleElementTypeDataMesh(): ...
