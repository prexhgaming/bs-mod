# module: FilterComponent

import sys

ApplyMotion: int = 0x80
CollisionFilterInfo: int = 0x88
DebugDraw: int = 0x83
DebugOutput: int = 0x81
DebugOutput2: int = 0x82
DriveRigidBody: int = 0x94
EnableDrop: int = 0x84
EnableOptimize: int = 0x8C
ForceDropTest: int = 0x8E
InputInterval: int = 0x90
OptimizeDelay: int = 0x8D
PlatformEntity: int = 0x98
UseHeightMap: int = 0x85

def GetFilterComponent(): ...
