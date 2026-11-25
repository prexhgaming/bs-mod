# module: RotateEasingInfo

import sys

easingEndPitchYaw: int = 0x10
easingFinished: int = 0x20
easingFinishedNextFrame: int = 0x21
lastEasingPitchYaw: int = 0x18

def GetRotateEasingInfo(): ...
