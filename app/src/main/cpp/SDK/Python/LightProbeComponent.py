# module: LightProbeComponent

import sys

AttenDistance: int = 0xDC
Enable: int = 0xD8
EnableTimeOfDay: int = 0xDA
IBL_Diffuse: int = 0xE4
IBL_SH: int = 0x12C
InShadow: int = 0xE0
IsDefault: int = 0xD9
TimeOfDays: int = 0x1A0

def GetLightProbeComponent(): ...
