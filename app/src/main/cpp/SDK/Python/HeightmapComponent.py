# module: HeightmapComponent

import sys

AccumulationSpeed: int = 0xDC
BaseMapTileSize: int = 0xBC
DetailMapTileSize: int = 0xC0
EnableVSHeightmap: int = 0xD4
FarTessDistance: int = 0xC8
FarTriPerMeter: int = 0xD0
MaxSnowLocalHeight: int = 0xD8
NearTessDistance: int = 0xC4
NearTriPerMeter: int = 0xCC
TextureSize: int = 0xBA

def GetHeightmapComponent(): ...
