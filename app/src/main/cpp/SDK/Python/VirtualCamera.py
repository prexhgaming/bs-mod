# module: VirtualCamera

import sys

BlendRatio: int = 0x94
EnableDebug: int = 0xB0
IsPostModify: int = 0x88
Reversed: int = 0x90
VCName: int = 0x98
VCPriority: int = 0x8C

def GetVirtualCamera(): ...
