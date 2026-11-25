# module: FoliageComponent

import sys

Clusters: int = 0x2A0
Compressed: int = 0x2B8
EnableInteraction: int = 0x339
IsCompressed: int = 0x337
IsDisableXRay: int = 0x338
IsLightmapConst: int = 0x33B
LastLodBillboard: int = 0x334
LodDistance0: int = 0x320
LodDistance1: int = 0x328
LodMeshTransitionTime: int = 0x330
LodTransition0: int = 0x324
LodTransition1: int = 0x32C
ReceiveDecals: int = 0x33A
TerrainOverlayBlend: int = 0x33C
Version: int = 0x340

def GetFoliageComponent(): ...
