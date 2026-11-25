# module: NoiseController

import sys

CurrentPosition: int = 0xE4
FinalPitch: int = 0x120
FinalPosition: int = 0x114
FinalRoll: int = 0x128
FinalYaw: int = 0x124
PositionNoise: int = 0x90
PositionNoiseFPS: int = 0xA8
PositionNoiseWeight: int = 0xAC
RotationNoise: int = 0xB8
RotationNoiseFPS: int = 0xD0
RotationNoiseWeight: int = 0xD4

def GetNoiseController(): ...
