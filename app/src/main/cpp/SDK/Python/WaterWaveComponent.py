# module: WaterWaveComponent

import sys

ChunkRes: int = 0x6E8
Collected: int = 0x75C
CullingBound: int = 0x718
CullingFlags: int = 0x700
DisplaceRange: int = 0x774
EnableSSR: int = 0x6FC
FocusPosition: int = 0x768
FrameIndex: int = 0x764
FrameTime: int = 0x760
GridRes: int = 0x6F4
HeightRange: int = 0x784
IsFoamAlphaReady: int = 0x759
IsFoamNormalReady: int = 0x75A
IsMaterialReady: int = 0x751
IsShorelineReady: int = 0x75B
IsTextureReady: int = 0x758
IsWaterWaveResourceReady: int = 0x750
LodRangeScale: int = 0x6F8
MaterialGuid: int = 0x7A8
NeedUpdateMesh: int = 0x730
NoFarPlaneClip: int = 0x6E4
RequiredTerrains: int = 0x790
SectorRes: int = 0x6EC
ShorelineBoundInfo: int = 0x7D8
ShorelineGuid: int = 0x7C8
ShorelineRangeInfo: int = 0x7E8
ShorelineWaveFoamGuid: int = 0x7F8
ShorelineWaveNoiseGuid: int = 0x808
Size: int = 0x6F0
TexturesReadyCount: int = 0x754
Viewpoints: int = 0x738
WaterWaveGuid: int = 0x7B8

def GetWaterWaveComponent(): ...
