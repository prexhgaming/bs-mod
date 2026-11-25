# module: DestructibleDebrisFilterData

import sys

DebrisLifetimeMax: int = 0x30
DebrisLifetimeMin: int = 0x2C
DebrisMaxSize: int = 0xC
DebrisNoCollisionLiftime: int = 0x34
UseValidBounds: int = 0x10
ValidBounds: int = 0x14

def GetDestructibleDebrisFilterData(): ...
