# module: CameraEasing

import sys

AdditiveAffiliatedFovOffsetEasingEscaped: int = 0x114
AdditiveAffiliatedFovOffsetEasingInfo: int = 0x154
AdditiveFovOffsetEasingEscaped: int = 0x110
AdditiveFovOffsetEasingInfo: int = 0x140
AffiliatedFovEasingEscaped: int = 0x10C
AffiliatedFovEasingInfo: int = 0x12C
AnimTarget: int = 0x200
BeginAffiliatedFov: int = 0x90
BeginFov: int = 0x88
BoneNameRotation: int = 0x1B8
BonePositionOffset: int = 0x1DC
BoneRotationOffset: int = 0x1E8
CurrAdditiveAffiliatedFovOffset: int = 0x9C
CurrAdditiveFovOffset: int = 0x98
CurrAffiliatedFov: int = 0x94
CurrFov: int = 0x8C
FovEasingEscaped: int = 0x108
FovEasingInfo: int = 0x118
LastBoneRotation: int = 0x1D0
RotateEasingEscaped: int = 0x188
RotateEasingInfo: int = 0x18C
UseAdditiveFovOffset: int = 0xA0
UseBoneRotation: int = 0x1B0

def GetCameraEasing(): ...
