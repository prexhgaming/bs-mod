# module: CameraRuntimeInfo

import sys

affiliatedFov: int = 0x20
controller: int = 0x28
direction: int = 0xC
fov: int = 0x1C
roll: int = 0x18

def GetCameraRuntimeInfo(): ...
