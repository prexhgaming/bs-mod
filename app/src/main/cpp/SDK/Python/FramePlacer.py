# module: FramePlacer

import sys

Cursor: int = 0xB0
Duration: int = 0xB8
Finished: int = 0xAD
Origin: int = 0xA0
Suspend: int = 0xAC
SuspendCursor: int = 0xB4

def GetFramePlacer(): ...
