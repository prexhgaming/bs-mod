# module: CameraCollider

import sys

CollisionFilterInfo: int = 0xBC
EnableCollider: int = 0xC8
EnableOverlap: int = 0xC9
EnableSmooth: int = 0xCB
EnableSweepEvent: int = 0xB9
EnableTargetCollider: int = 0xCA
InvalidCollideInfo: int = 0xD8
InvalidTargetCollideInfo: int = 0xD9
MinDist: int = 0xC0
OffsetWallSticking: int = 0xC4
OverlapFilterInfo: int = 0xCC
SmoothRatio: int = 0xD4
SmoothThreshold: int = 0xD0
SweepCollided: int = 0xB8
SweepShape: int = 0x170
TerrainFilterInfo: int = 0x178

def GetCameraCollider(): ...
