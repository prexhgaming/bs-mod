# module: GarmentResource

import sys

CollisionSkinningMesh: int = 0x188
DihBends: int = 0xA8
DihBendsIndex: int = 0x88
GarmentRenderMeshs: int = 0x218
GarmentSkinningMesh: int = 0x110
IsoBends: int = 0x70
IsoBendsIndex: int = 0x50
Masks: int = 0xF8
SeamGroup: int = 0xE0
SeamGroupIndex: int = 0xC0
StaticBound: int = 0x200
Stretchs: int = 0x38
StretchsIndex: int = 0x18

def GetGarmentResource(): ...
