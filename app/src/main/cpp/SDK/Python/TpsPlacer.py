# module: TpsPlacer

import sys

AdditiveBoneName: int = 0xD8
AdditiveFovOffset: int = 0x1E4
AdditiveRollOffset: int = 0x1EC
AffiliatedFov: int = 0x1E0
AlreadyFirstRotate: int = 0x1F0
AngularSpeed: int = 0x1C0
BoneName: int = 0xC0
Direction: int = 0x200
Easing: int = 0x108
EasingRuntimeInfo: int = 0x110
EnableFaceCheck: int = 0xF8
EnableInertia: int = 0x194
EnableRotate: int = 0x1B8
EnableUpPos: int = 0x148
FaceThreshold: int = 0xFC
FocusDiffTol: int = 0x19C
FocusPos: int = 0x178
Fov: int = 0x1DC
InertiaCoeff: int = 0x184
InertiaCoeffAccFrame: int = 0x1A4
InertiaCoeffAccReset: int = 0x1A8
InertiaCoeffAccStart: int = 0x1AC
InertiaCoeffAccType: int = 0x1B4
InertiaCoeffCurrent: int = 0x1B0
InertiaCoeffDirection: int = 0x188
InertiaCoeffHorizon: int = 0x1A0
InertiaHalfLife: int = 0x18C
InertiaType: int = 0x198
Mover: int = 0x100
PitchMax: int = 0x160
PitchMin: int = 0x15C
PrevDirection: int = 0x20C
PrevTargetPos: int = 0x1C4
Roll: int = 0x1E8
Target: int = 0x218
TargetDiffTol: int = 0x1BC
TargetPos: int = 0x1D0
TargetPosOffset: int = 0x1F4
TargetTransform: int = 0x220
UseBonePosition: int = 0xB8
ZoomHalfLife: int = 0x150
ZoomMax: int = 0x158
ZoomMin: int = 0x154

def GetTpsPlacer(): ...
