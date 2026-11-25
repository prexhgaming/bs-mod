# module: VolumeComponent

import sys

ControlPoints: int = 0x180
IsInVolume: int = 0x149
IsInVolumeBound: int = 0x148
LocalBound: int = 0x14C
ShapeType: int = 0x17C
World: int = 0x140
WorldBound: int = 0x164

def GetVolumeComponent(): ...
