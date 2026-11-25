# module: WaterWaveInfo

import sys

Amplitude: int = 0x24
ChoppyScale: int = 0x34
DirBias: int = 0x40
DirMovement: int = 0x44
DisplaceRanges: int = 0x98
DisplacementMaps: int = 0xB0
FoamAlphaGuid: int = 0x70
FoamAmount: int = 0x48
FoamDissipationSpeed: int = 0x50
FoamFallOffPower: int = 0x58
FoamFallOffSpeed: int = 0x54
FoamNormalGuid: int = 0x80
FoamThreshold: int = 0x4C
FrameCount: int = 0x60
FrameIntervalTime: int = 0x64
FrameIntervalTimeInv: int = 0x68
GridSize: int = 0x20
Length: int = 0x1C
Noise: int = 0x3C
NormalMaps: int = 0xF8
PeriodCount: int = 0x5C
PlayPeriod: int = 0x90
SlopeRanges: int = 0xE0
SmallCutOff: int = 0x28
UVScale: int = 0x6C
UnitPeriod: int = 0x38
WindAngle: int = 0x30
WindStrength: int = 0x2C

def GetWaterWaveInfo(): ...
