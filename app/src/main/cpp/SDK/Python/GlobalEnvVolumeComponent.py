# module: GlobalEnvVolumeComponent

import sys

ActiveVolume: int = 0xC48
AutoExposureMeterMask: int = 0xB00
BloomDirtMask: int = 0xB10
BlueNoiseMap: int = 0xBA0
Cloud3DNoiseMap: int = 0xB30
CloudDetail3DNoiseMap: int = 0xB40
CloudWeatherMap: int = 0xB20
DetailFogAlbedoA: int = 0xBEC
DetailFogAlbedoB: int = 0xBF8
DetailFogCoverage: int = 0xBD0
DetailFogCutoff: int = 0xBE8
DetailFogMaxHeight: int = 0xBE4
DetailFogMinHeight: int = 0xBE0
FadeOutTime: int = 0xC3C
FadeOutType: int = 0xC40
FogWeatherMap: int = 0xBC0
LenFlares: int = 0xB68
LensFadeTime: int = 0xB60
LensMap: int = 0xB50
MoonMap: int = 0xBB0
MusicEventParam: int = 0xC08
NoiseEventParam: int = 0xC20
RainNormalMap: int = 0xB80
SkinProfileMap: int = 0xB90
VolumeScale: int = 0xC38

def GetGlobalEnvVolumeComponent(): ...
