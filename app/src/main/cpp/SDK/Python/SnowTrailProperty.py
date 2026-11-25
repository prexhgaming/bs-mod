# module: SnowTrailProperty

import sys

ElevationHeightScale: int = 0x34
ElevationRangeScale: int = 0x30
Exponent: int = 0x2C
InnerRadius: int = 0x24
Offset: int = 0x18
OuterRadius: int = 0x28
Randomness: int = 0x38

def GetSnowTrailProperty(): ...
