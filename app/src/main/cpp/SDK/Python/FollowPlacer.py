# module: FollowPlacer

import sys

AngularSpeed: int = 0x138
BoneName: int = 0xC0
Direction: int = 0x168
EnableFaceCheck: int = 0xE0
EnableInertia: int = 0x128
EnableRotate: int = 0x130
EnableUpPos: int = 0xF0
EnableZoomSmooth: int = 0x1B0
FaceThreshold: int = 0xE4
FocusDiffTol: int = 0x12C
FocusPos: int = 0x118
Fov: int = 0x154
InertiaCoeff: int = 0x124
Mover: int = 0xE8
PitchMax: int = 0x108
PitchMin: int = 0x104
PrevTargetPos: int = 0x13C
Roll: int = 0x158
Target: int = 0x178
TargetDiffTol: int = 0x134
TargetPos: int = 0x148
TargetPosOffset: int = 0x15C
TargetTransform: int = 0x180
UseBonePosition: int = 0xB8
ZoomHalfLife: int = 0xF8
ZoomMax: int = 0x100
ZoomMin: int = 0xFC

def GetFollowPlacer(): ...
