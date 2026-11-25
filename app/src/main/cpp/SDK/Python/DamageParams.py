# module: DamageParams

import sys

ChunkIndex0: int = 0x84
ChunkIndex1: int = 0x88
Damage: int = 0x74
DamageType: int = 0x40
ForceBreak: int = 0xA8
Groups: int = 0x90
MaxRadius: int = 0x7C
MinRadius: int = 0x78
Normal: int = 0x68
Point0: int = 0x44
Point1: int = 0x50
Point2: int = 0x5C
Radius: int = 0x80

def GetDamageParams(): ...
