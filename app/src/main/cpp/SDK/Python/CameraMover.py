# module: CameraMover

import sys

BeginDirection: int = 0xC4
BeginFov: int = 0x100
BeginTargetPosOffset: int = 0xB8
BoneName: int = 0x90
CurrDirection: int = 0xF4
CurrFov: int = 0x108
CurrTargetPosOffset: int = 0xE8
EndDirection: int = 0xDC
EndFov: int = 0x104
EndTargetPosOffset: int = 0xD0
Finished: int = 0x10C
MoveType: int = 0xA8
Target: int = 0x120
TargetPos: int = 0x110
TargetTransform: int = 0x128
UseBonePosition: int = 0x88

def GetCameraMover(): ...
