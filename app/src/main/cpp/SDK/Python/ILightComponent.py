# module: ILightComponent

import sys

CastDynamicShadow: int = 0xB0
CastShadow: int = 0x9D
CastVolumetricShadow: int = 0xA4
Channel: int = 0xD8
Enable: int = 0x98
EnableVolumetricFog: int = 0x9F
FlareNodes: int = 0xB8
IsBake: int = 0x9A
IsBakeGIOnly: int = 0x9B
IsDynamic: int = 0x99
IsHybridBake: int = 0x9C
IsImportant: int = 0xB2
LensFadeTime: int = 0xD0
OccluId: int = 0xD4
PBRQuality: int = 0x9E
SSSTranslucency: int = 0xA5
ShadowBias: int = 0xA8
ShadowQuality: int = 0xB1
ShadowSlopeBias: int = 0xAC
VolumetricScatteringIntensity: int = 0xA0

def GetILightComponent(): ...
