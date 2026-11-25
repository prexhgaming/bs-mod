# module: SPSVolumeData

import sys

SizeX: int = 0x20
SizeY: int = 0x24
SizeZ: int = 0x28
TileCounts: int = 0x2C
TileHeight: int = 0x1C
TileSize: int = 0x18

def GetSPSVolumeData(): ...
