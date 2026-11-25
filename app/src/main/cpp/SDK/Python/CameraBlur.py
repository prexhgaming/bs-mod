# module: CameraBlur

import sys

Enabled: int = 0x98
FarTransitionRegion: int = 0xA0
FocalRegion: int = 0x9C
MaxBlurriness: int = 0x88
MaxDirLen: int = 0x90
MinBlurriness: int = 0x8C
MinDirLen: int = 0x94

def GetCameraBlur(): ...
