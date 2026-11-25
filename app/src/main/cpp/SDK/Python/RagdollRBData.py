# module: RagdollRBData

import sys

ColProp: int = 0x40
DynProp: int = 0x64
InitPose: int = 0x7C
MassProp: int = 0x14
MotionType: int = 0xAC
Shapes: int = 0xB0

def GetRagdollRBData(): ...
