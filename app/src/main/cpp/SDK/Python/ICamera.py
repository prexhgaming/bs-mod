# module: ICamera

import sys

Blurriness: int = 0x354
EnableDepthOfField: int = 0x301
Far: int = 0x340
FarTransitionRegion: int = 0x350
FieldOfView: int = 0x338
FocalDistance: int = 0x344
FocalRegion: int = 0x348
IsOrthographic: int = 0x300
Near: int = 0x33C
NearTransitionRegion: int = 0x34C
OrthoScale: int = 0x334
ViewMatrix: int = 0x304
VirtualCameraManager: int = 0x2F8

def GetICamera(): ...
