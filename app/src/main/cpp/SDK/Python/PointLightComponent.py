# module: PointLightComponent

import sys

Color: int = 0x13C
FilterRange: int = 0x160
IESTexture: int = 0x168
Intensity: int = 0x14C
InverseSquaredFalloff: int = 0x154
IsSphericalLight: int = 0x155
Lumens: int = 0x150
Radius: int = 0x15C
Range: int = 0x158
Temperature: int = 0x148

def GetPointLightComponent(): ...
