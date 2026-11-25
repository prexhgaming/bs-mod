# module: FoliageClusterCompressed

import sys

LightMapScaleMax: int = 0x88
LightMapScaleMin: int = 0x78
LightMapScales: int = 0x98
PitchRolls: int = 0x48
Positions: int = 0x18
Scales: int = 0x60
Yaws: int = 0x30

def GetFoliageClusterCompressed(): ...
