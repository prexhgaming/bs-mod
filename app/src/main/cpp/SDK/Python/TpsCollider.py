# module: TpsCollider

import sys

CollisionFilterInfo: int = 0x88
EnableCollider: int = 0x90
EnableSmoothIn: int = 0x92
EnableSmoothOut: int = 0x91
InvalidCollideInfo: int = 0x9C
MinDist: int = 0x8C
SmoothRatio: int = 0x98
SmoothThreshold: int = 0x94
SweepShape: int = 0xE0
Target: int = 0x108
TargetPos: int = 0xEC
TargetPosOffset: int = 0xF8
TerrainFilterInfo: int = 0xE8
UseCameraTargetAsStartPoint: int = 0x104

def GetTpsCollider(): ...
