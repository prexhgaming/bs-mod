# module: ReflectionProbeComponent

import sys

AsSky: int = 0x16A
AttenDistance: int = 0x180
Enable: int = 0x168
EnableTimeOfDay: int = 0x16B
Guid: int = 0x148
GuidArray: int = 0x158
IBL_Diffuse: int = 0x188
IBL_SH: int = 0x1D0
InShadow: int = 0x16C
IsDefault: int = 0x169
MipLayers: int = 0x184
OffsetPos: int = 0x170
ReflectionDistance: int = 0x17C
TimeOfDays: int = 0x240

def GetReflectionProbeComponent(): ...
