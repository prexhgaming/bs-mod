# module: CameraFrame

import sys

Fov: int = 0x18
InterpolateMode: int = 0x1C
Pitch: int = 0xC
Roll: int = 0x14
Time: int = 0x20
Yaw: int = 0x10

def GetCameraFrame(): ...
