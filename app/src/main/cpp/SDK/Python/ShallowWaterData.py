# module: ShallowWaterData

import sys

BorderSize: int = 0x34
BoundaryDamping: int = 0x38
BoundarySurfaceHeight: int = 0x30
DeltaTime: int = 0x2C
DepthDecayRate: int = 0x54
ForceByVelY: int = 0x40
GridLength: int = 0x14
GridSize: int = 0xC
HeightChange: int = 0x58
IsOpenSeaBoundary: int = 0x3C
LandGridLength: int = 0x28
LandGridSize: int = 0x20
LandPosOffset: int = 0x18
Lift: int = 0x4C
LinearDrag: int = 0x44
Omega: int = 0x50
QuadDrag: int = 0x48
VelChange: int = 0x5C

def GetShallowWaterData(): ...
