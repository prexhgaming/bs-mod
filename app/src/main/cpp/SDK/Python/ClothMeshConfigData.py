# module: ClothMeshConfigData

import sys

AirDensityScale: int = 0x68
AirDragCoefficient: int = 0x60
AirLiftCoefficient: int = 0x64
AirMeshVolumeScale: int = 0xE4
AngularDrag: int = 0xA8
AngularInertia: int = 0xC0
AnimDriveDamping: int = 0x74
AnimDriveMultiplier: int = 0x70
CentrifugalInertia: int = 0xCC
ClothWindMethodType: int = 0x7C
CollisionMassScale: int = 0x8C
CollisionThickness: int = 0xE8
CompressionLimit: int = 0x38
CpuTangentUpdate: int = 0x91
Damping: int = 0x54
EnableCCD: int = 0x90
Friction: int = 0x88
GravityScale: int = 0x80
InvMassToArea: int = 0x78
LinearDrag: int = 0x9C
LinearInertia: int = 0xB4
MassScale: int = 0x84
SelfCollisionCullScale: int = 0x50
SelfCollisionStiffness: int = 0x4C
SelfCollisionThickness: int = 0x48
ShapeMatchingFactor: int = 0xE0
SolverFreq: int = 0x6C
StiffnessFreq: int = 0x98
StiffnessMultiplier: int = 0x18
StretchLimit: int = 0x28
TetherLimit: int = 0xDC
TetherStiffness: int = 0xD8
VirtualParticleLevel: int = 0x94

def GetClothMeshConfigData(): ...
