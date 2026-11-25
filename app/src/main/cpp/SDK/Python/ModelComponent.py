# module: ModelComponent

import sys

BatchKey: int = 0x3D0
Distance: int = 0x2F4
EnvMap: int = 0x3A0
HideInMainView: int = 0x3D5
InScreenDepthMask: int = 0x3D4
IsDeformable: int = 0x3CD
IsEnvMapReady: int = 0x361
IsHiddenUntilReady: int = 0x3F8
IsLightmapConst: int = 0x3CC
IsLightmapReady: int = 0x360
Lightmap: int = 0x368
LightmapAlwaysLoad: int = 0x398
LightmapOffset: int = 0x388
LightmapScale: int = 0x378
LodFadingTime: int = 0x288
NoFarPlaneClip: int = 0x362
ReceiveDecals: int = 0x363
ReflectionBox: int = 0x3B0
ReflectionProbeLum: int = 0x3C8
SkinCacheMode: int = 0x3D6
UseCustomMaterialOnly: int = 0x3CE

def GetModelComponent(): ...
