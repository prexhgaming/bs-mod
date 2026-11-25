# module: TerrainVolumeComponent

import sys

Material: int = 0x310
MeshLodDistance: int = 0x324
MeshUpdateThreshold: int = 0x320
UseObservationPoints: int = 0x328

def GetTerrainVolumeComponent(): ...
