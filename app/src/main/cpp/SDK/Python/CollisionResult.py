# module: CollisionResult

import sys

AuxData: int = 0x58
Body: int = 0x38
Distance: int = 0x4C
Flags: int = 0x50
IsHit: int = 0x48
MaterialTypeId: int = 0x54
Normal: int = 0x28
Shape: int = 0x40

def GetCollisionResult(): ...
