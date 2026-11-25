# module: CameraMotor

import sys

BackToStart: int = 0xD4
CurrMoveVector: int = 0xA0
CurrRotateAngle: int = 0xC8
EndFrame: int = 0xB4
EscapedTime: int = 0xD8
Finished: int = 0x90
FrameCount: int = 0xB8
InterpolateMode: int = 0x8C
Invalid: int = 0x88
MoveVector: int = 0x94
MoveVectorLen: int = 0xB0
MoveVectorType: int = 0xAC
RotateAngle: int = 0xBC
TotalTime: int = 0xDC

def GetCameraMotor(): ...
