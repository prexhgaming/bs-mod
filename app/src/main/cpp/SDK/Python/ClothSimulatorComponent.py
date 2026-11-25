# module: ClothSimulatorComponent

import sys

ColShapeAssetPath: int = 0xF8
CollisionFilterInfo: int = 0x14C
CollisionModifier: int = 0x130
ControlBoneName: int = 0x170
ControlledByMutiBone: int = 0x16A
CouplingConfig: int = 0x1F0
CouplingRigidBodyInOut: int = 0x208
EnableInterClothAirmesh: int = 0x14A
EnablePhysicsSpaceCollision: int = 0x14B
EnableSelfAirmesh: int = 0x149
Gravity: int = 0x110
KeepAnimShape: int = 0x12C
LeftShoulder: int = 0x188
LinearSpeedSensitivity: int = 0x128
LowFpsProtection: int = 0x158
MaxBoundMovement: int = 0x160
MaxDeltaRotation: int = 0x154
MaxDeltaTranslation: int = 0x150
MaxDisScale: int = 0x15C
RemoveMotion: int = 0x148
ResetOnce: int = 0x169
RightShoulder: int = 0x1A0
SolverFreqOverride: int = 0x1BC
WaitAnimated: int = 0x1B8
Wind: int = 0x11C

def GetClothSimulatorComponent(): ...
