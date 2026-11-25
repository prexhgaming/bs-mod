# module: BonePlacer

import sys

BoneName: int = 0xA8
Direction: int = 0xD8
Finished: int = 0x9C
Fov: int = 0xA0
Height: int = 0x98
PlacerPos: int = 0xCC
Radius: int = 0x94
Skeleton: int = 0x88
Target: int = 0xE8
TargetPos: int = 0xC0
Yaw: int = 0x90

def GetBonePlacer(): ...
