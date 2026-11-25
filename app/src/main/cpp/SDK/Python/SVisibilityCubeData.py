# module: SVisibilityCubeData

import sys

EffectBitCount: int = 0x2C
EffectBytesPerTile: int = 0x30
MaxOccluId: int = 0x28
SampleCount: int = 0x40
SizeX: int = 0x1C
SizeY: int = 0x20
SizeZ: int = 0x24
TileSize: int = 0x18

def GetSVisibilityCubeData(): ...
