# module: SpineComponent

import sys

AmbientLightScale: int = 0xD8
LightingEnable: int = 0xDE
LockY: int = 0xDC
PointLightScale: int = 0xD0
SpotLightScale: int = 0xD4
SunLightScale: int = 0xCC
TimeScale: int = 0xC8
WriteDepth: int = 0xDD

def GetSpineComponent(): ...
