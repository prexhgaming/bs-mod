# module: TpsMover

import sys

EscapedTime: int = 0x174
Invalid: int = 0x178
MoveTime: int = 0x170
PitchMax: int = 0x16C
PitchMin: int = 0x168
PitchTolerance: int = 0x164
TweenMode: int = 0x179
YawTolerance: int = 0x160

def GetTpsMover(): ...
