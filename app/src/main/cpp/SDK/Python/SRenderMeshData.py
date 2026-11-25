# module: SRenderMeshData

import sys

BoundingBox: int = 0x34
BoundingVolume: int = 0x18
Groups: int = 0xF0
IndexCount: int = 0xC
Indices: int = 0xD0
KeepData: int = 0x15
PrimTopology: int = 0x14
StreamingMask: int = 0x10
Streams: int = 0x50

def GetSRenderMeshData(): ...
