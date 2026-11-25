# module: SSHVolumeData

import sys

AxisX: int = 0xC
AxisY: int = 0x18
AxisZ: int = 0x24
BGridSizeX: int = 0x5C
BGridSizeY: int = 0x60
BGridSizeZ: int = 0x64
BGridTileSize: int = 0x58
CGridSizeX: int = 0x6C
CGridSizeY: int = 0x70
CGridSizeZ: int = 0x74
CGridTileSize: int = 0x68
D: int = 0x54
L: int = 0x50
LenXYZ: int = 0x30
NPoints: int = 0x4C
SizeX: int = 0x40
SizeY: int = 0x44
SizeZ: int = 0x48
TileSize: int = 0x3C

def GetSSHVolumeData(): ...
