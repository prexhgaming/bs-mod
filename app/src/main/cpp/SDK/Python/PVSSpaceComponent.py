# module: PVSSpaceComponent

import sys

CameraHeight: int = 0x9C
PVSCubes: int = 0xD0
PVSSunShadow: int = 0xE8
TileHeight0: int = 0xA0
TileHeight1: int = 0xA4
TileHeight2: int = 0xA8
TileSize0: int = 0x90
TileSize1: int = 0x94
TileSize2: int = 0x98
WorldBB: int = 0xAC
XSize: int = 0xC4
ZSize: int = 0xC8

def GetPVSSpaceComponent(): ...
