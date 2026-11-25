# module: AreaLightComponent

import sys

DoubleSide: int = 0x61
Enable: int = 0x60
GIOnly: int = 0x78
Intensity: int = 0x70
LightColor: int = 0x64
Range: int = 0x74

def GetAreaLightComponent(): ...
