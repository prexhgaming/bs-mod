# module: AnimatedController

import sys

CustomAspect: int = 0xB8
FieldOfView: int = 0xD4
IsPlaying: int = 0xA8
PauseEnd: int = 0xA9
Pitch: int = 0xC8
PlayedTime: int = 0xAC
Playrate: int = 0xB0
Position: int = 0xBC
Roll: int = 0xD0
VerticalFOV: int = 0xB4
Yaw: int = 0xCC

def GetAnimatedController(): ...
