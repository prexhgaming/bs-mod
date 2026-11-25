# module: SlavedPlacer

import sys

AngularSpeed: int = 0xF0
CameraVelocity: int = 0xE0
Direction: int = 0x11C
EnableRotate: int = 0xEC
EnableUpPos: int = 0xB8
EnableYawTrace: int = 0x13C
FocusPos: int = 0xD4
Fov: int = 0x10C
FreeRadius: int = 0x1A0
InputLock: int = 0x154
MotionDamping: int = 0x12C
MotionStiffness: int = 0x128
Mover: int = 0x1A8
PitchMax: int = 0xD0
PitchMin: int = 0xCC
PrevTargetPos: int = 0xF4
RotationDamping: int = 0x134
RotationStiffness: int = 0x130
RotationThreshold: int = 0x138
Target: int = 0x1B0
TargetPos: int = 0x100
TargetPosOffset: int = 0x110
TargetTransform: int = 0x1B8
YawTraceRadius: int = 0x140
ZoomHalfLife: int = 0xC0
ZoomMax: int = 0xC8
ZoomMin: int = 0xC4

def GetSlavedPlacer(): ...
