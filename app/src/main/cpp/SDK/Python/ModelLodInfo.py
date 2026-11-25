# module: ModelLodInfo

import sys

BoundingVolume: int = 0x1C
LodDistance: int = 0x38
LodScreenSize: int = 0x50
ModelFlags: int = 0x80
ModelGuids: int = 0x68

def GetModelLodInfo(): ...
