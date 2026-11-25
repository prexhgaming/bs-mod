# module: RBStateSyncerDebugComponent

import sys

BlendInColor: int = 0x208
BlendOutColor: int = 0x214
BlendPointBufferSize: int = 0x261
EstColor: int = 0x1FC
EstPointBufferSize: int = 0x260
TrajColor: int = 0x1F0
TrajPointBufferSize: int = 0x262

def GetRBStateSyncerDebugComponent(): ...
