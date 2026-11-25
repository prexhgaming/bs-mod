# module: PSVolumeSpaceComponent

import sys

PSVolumes: int = 0xB8
TileHeight: int = 0x94
TileSize: int = 0x90
WorldBB: int = 0x98
XSize: int = 0xB0
ZSize: int = 0xB4

def GetPSVolumeSpaceComponent(): ...
