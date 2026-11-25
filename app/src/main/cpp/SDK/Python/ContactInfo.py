# module: ContactInfo

import sys

AbsPos: int = 0x58
Flag: int = 0x48
Impulse: int = 0x7C
MaterialTypeIdA: int = 0x40
MaterialTypeIdB: int = 0x44
Normal: int = 0x70
Pos: int = 0x4C
ShapeA: int = 0x30
ShapeB: int = 0x38

def GetContactInfo(): ...
