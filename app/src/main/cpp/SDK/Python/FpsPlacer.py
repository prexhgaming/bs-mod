# module: FpsPlacer

import sys

AdditiveAffiliatedFovOffset: int = 0x124
AdditiveBoneName: int = 0xA8
AdditiveFovOffset: int = 0x120
AdditiveRollOffset: int = 0x12C
AffiliatedFov: int = 0x11C
ApplyPivotRotate: int = 0x164
BasePosition: int = 0x134
BoneName: int = 0x90
Direction: int = 0x158
Easing: int = 0xD0
EasingRuntimeInfo: int = 0xD8
Fov: int = 0x118
Mover: int = 0xC8
PitchMax: int = 0x114
PitchMin: int = 0x110
PivotDist: int = 0x130
PivotOffset: int = 0x14C
PositionOffset: int = 0x140
Roll: int = 0x128
Target: int = 0x168
UseBonePosition: int = 0x88

def GetFpsPlacer(): ...
