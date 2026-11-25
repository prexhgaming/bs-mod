# module: PivotPlacer

import sys

AngularRatio: int = 0xE4
AngularTolerance: int = 0xF4
Aspect: int = 0x11C
Direction: int = 0x138
EscapedTime: int = 0xD8
Finished: int = 0xC0
FocusPosOffset: int = 0x12C
FocusRatio: int = 0xE8
FocusTolerance: int = 0xF8
Fov: int = 0x118
HalfFovH: int = 0x120
HalfFovV: int = 0x124
InsideScreen: int = 0xE2
LinkAngle: int = 0x10C
LinkAngleEpsilon: int = 0x104
LinkAngleThreshold: int = 0x108
MajorTarget: int = 0x178
MajorTargetPos: int = 0x144
MajorTargetPosOffset: int = 0x15C
MajorTargetTransform: int = 0x188
ManualRotate: int = 0xC1
MaxDistance: int = 0xB8
MinDistXz: int = 0x110
MinLinkDist: int = 0xBC
MinorTarget: int = 0x180
MinorTargetPos: int = 0x150
MinorTargetPosOffset: int = 0x168
MinorTargetTransform: int = 0x1B8
PitchAngle: int = 0x114
PitchMax: int = 0xC8
PitchMin: int = 0xC4
PivotRatio: int = 0x128
RadialRatio: int = 0xEC
RadialTolerance: int = 0xFC
ResetPhase: int = 0xE0
TotalTime: int = 0xDC
UseScreenAngle: int = 0xE1
VerticalRatio: int = 0xF0
VerticalTolerance: int = 0x100
ZoomHalflife: int = 0xD4
ZoomRatio: int = 0xD0

def GetPivotPlacer(): ...
