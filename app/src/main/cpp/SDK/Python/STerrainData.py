# module: STerrainData

import sys

ChunkSampleResolution: int = 0xE
ChunkSize: int = 0x10
Chunks: int = 0x18
Flags: int = 0x30
MeshResolution: int = 0xC

def GetSTerrainData(): ...
