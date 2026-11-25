# module: AimPlacer

import sys

AimDirection: int = 0xF8
AimDirectionPitchMax: int = 0x104
AimDirectionPitchMin: int = 0x108
Direction: int = 0xEC
EnableFocusInertia: int = 0xBD
EnableInertia: int = 0xBC
ExpRatio: int = 0xB8
Finished: int = 0xA0
FocusInertiaCoeff: int = 0xC0
FocusInertiaMaxLength: int = 0xC4
FocusPos: int = 0xC8
Fov: int = 0xA4
InertiaCoeff: int = 0xB0
InertiaTol: int = 0xAC
InertiaType: int = 0xA8
ManualAdjust: int = 0x94
MaxYawDelta: int = 0xB4
PitchMax: int = 0x9C
PitchMin: int = 0x98
Target: int = 0x110
TargetPos: int = 0xD4
TargetPosOffset: int = 0xE0
TargetTransform: int = 0x118

def GetAimPlacer(): ...
