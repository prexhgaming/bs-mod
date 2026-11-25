# module: ClothMeshStaticData

import sys

AnchorTable: int = 0xD8
ClothBoneCounts: int = 0x2D0
ClothBoneIndices: int = 0x2A0
ClothBoneWeights: int = 0x288
ClothCollisionCache: int = 0x120
ClothIndices: int = 0x258
ClothInvMass: int = 0x270
ClothPositions: int = 0x210
ClothTPosePositions: int = 0x240
ClothTable: int = 0x80
ClothUvs: int = 0x228
Configs: int = 0x20
ConstrainData: int = 0x38
RenderIndices: int = 0x1E0
RenderPositions: int = 0x1C8
RenderUvs: int = 0x1F8
WrapTable: int = 0xC0

def GetClothMeshStaticData(): ...
