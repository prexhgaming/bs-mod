# module: WFDebugDrawer

import sys

BottomScale: int = 0x7C
Color: int = 0x64
ShowFieldIndicator: int = 0x60
ShowFieldSlice: int = 0x80
SliceLevel: int = 0x84
VelAtMaxLength: int = 0x78
VelAtMinLength: int = 0x74

def GetWFDebugDrawer(): ...
