# module: DestructibleModelComponent

import sys

DModelPath: int = 0x288
IsLightmapConst: int = 0x390
IsLightmapReady: int = 0x391
Lightmap: int = 0x2A0
LightmapOffset: int = 0x380
LightmapScale: int = 0x370

def GetDestructibleModelComponent(): ...
