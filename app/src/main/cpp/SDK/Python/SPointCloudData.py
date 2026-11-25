# module: SPointCloudData

import sys

BGridSizeX: int = 0x3C
BGridSizeY: int = 0x40
BGridSizeZ: int = 0x44
BGridTileSize: int = 0x38
CGridSizeX: int = 0x4C
CGridSizeY: int = 0x50
CGridSizeZ: int = 0x54
CGridTileSize: int = 0x48
D: int = 0x34
L: int = 0x30
NPoints: int = 0x28
NSamples: int = 0x2C
SizeX: int = 0x1C
SizeY: int = 0x20
SizeZ: int = 0x24
TileSize: int = 0x18

def GetSPointCloudData(): ...
