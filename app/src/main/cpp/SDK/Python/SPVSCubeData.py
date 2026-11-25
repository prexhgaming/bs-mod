# module: SPVSCubeData

import sys

BytesPerTile: int = 0x2C
IsFourByteId: int = 0x34
MaxOccluID: int = 0x30
SizeX: int = 0x1C
SizeZ: int = 0x20
TileCounts: int = 0x28
TileHeight: int = 0x24
TileSize: int = 0x18
Version: int = 0x38

def GetSPVSCubeData(): ...
