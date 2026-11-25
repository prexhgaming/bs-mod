# module: ShallowWaterComponent

import sys

BoundaryDamping: int = 0x330
BoundaryHeight: int = 0x32C
BoundarySize: int = 0x328
EnableLOD: int = 0x300
FieldSize: int = 0x2D0
FollowTarget: int = 0x2F8
GridLength: int = 0x2D8
InitState: int = 0x388
IsOpenBoundary: int = 0x324
LandFieldSize: int = 0x2E4
LandGridLength: int = 0x2EC
LandPosOffset: int = 0x2DC
LodCheckPeriod: int = 0x320
LodDistance: int = 0x308
LodLevelLocked: int = 0x378
TargetLodLevel: int = 0x37C
WaterWaveDispScale: int = 0x360
WaterWaveGuid: int = 0x348
WaterWaveUVScale: int = 0x358
WorldPos: int = 0x2C4

def GetShallowWaterComponent(): ...
