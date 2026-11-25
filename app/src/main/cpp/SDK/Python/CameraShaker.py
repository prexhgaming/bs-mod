# module: CameraShaker

import sys

Amplitude: int = 0x8C
DecayCoeff: int = 0xA0
Duration: int = 0x98
Frequency: int = 0x94
LoopCount: int = 0x9C
Random: int = 0xB4
RollAmplitude: int = 0x90
ShakeDirection: int = 0xA8
ShakeType: int = 0xA4

def GetCameraShaker(): ...
