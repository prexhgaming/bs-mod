# module: EasingRuntimeInfo

import sys

affiliatedFovEasingFinished: int = 0x34
affiliatedFovOffsetEasingFinished: int = 0x35
currAdditiveAffiliatedFovOffset: int = 0xC
currBonePositionOffset: int = 0x10
currBoneRotationOffset: int = 0x1C
currPitchYawDelta: int = 0x28
fovEasingFinished: int = 0x32
fovOffsetEasingFinished: int = 0x33
rotateEasingFinished: int = 0x36
useAdditiveFovOffset: int = 0x31
useBoneRotation: int = 0x30

def GetEasingRuntimeInfo(): ...
