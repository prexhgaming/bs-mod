# module: TerrainComponent

import sys

AliasingFactor: int = 0x484
AlignToChunk: int = 0x4BE
AuxiliaryMap: int = 0x6D46046000
AuxiliaryMaps: int = 0x448
BlendSharpness: int = 0x480
BlockMap: int = 0x408
BlockNumber: int = 0x464
BlockScale: int = 0x460
BlockTags: int = 0x468
BloomScale: int = 0x4C4
ChunkSize: int = 0x3A8
Chunks: int = 0x3B0
DisplacementHeight: int = 0x48C
EnableOptimization: int = 0x4BC
ForbidRotate: int = 0x4BD
GeometryFactor: int = 0x4B0
IsDisableXRay: int = 0x4C0
LightmapScale: int = 0x3F8
Lightmaps: int = 0x3E0
Materials: int = 0x3C8
MaxDetailsDistance: int = 0x4B4
MaxLoadingDistance: int = 0x4B8
MixtureMap: int = 0x428
NormalMap: int = 0x418
OverlayMap: int = 0x438
Position: int = 0x398
Resolution: int = 0x3A0
ShaderType: int = 0x498
TessFadeBegin: int = 0x490
TessFadeEnd: int = 0x494
TrianglePerPixel: int = 0x488
UseObservationPoints: int = 0x4BF

def GetTerrainComponent(): ...
