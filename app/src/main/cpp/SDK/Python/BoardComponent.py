# module: BoardComponent

import sys

BoneName: int = 0xA8
BoundingBox: int = 0x98
ComplementalY: int = 0x78
DepthEnable: int = 0x6A
Displacement: int = 0x7C
FovDistance: int = 0x70
FrameEnable: int = 0x6B
IsAbsoluteDisplacement: int = 0x6C
IsRectConstrainted: int = 0x6D
RectMargin: int = 0x88
TopStick: int = 0x68
Visibility: int = 0x69
VisibleDistance: int = 0x74

def GetBoardComponent(): ...
