# module: OrbitPlacer

import sys

Cursor: int = 0xA0
Direction: int = 0x11C
Finished: int = 0x105
Fov: int = 0x108
FovEnabled: int = 0x106
Loop: int = 0x104
OrbitDuration: int = 0xA4
OrbitResourcePath: int = 0x88
Origin: int = 0xF8
Roll: int = 0x10C
RotateYaw: int = 0xF4
StartTime: int = 0xE4
Suspend: int = 0xA8
SuspendCursor: int = 0xAC
Target: int = 0x150
TargetPos: int = 0x110
TargetTransform: int = 0x158
TotalTime: int = 0xE0
YawPivot: int = 0xE8

def GetOrbitPlacer(): ...
