# module: ParticleElementCollision

import sys

CollideDistance: int = 0x58
Elasticity: int = 0x54
FilterInfo: int = 0x50
KillVelocity: int = 0x5C
RecordResult: int = 0x61
SimpleCollision: int = 0x60

def GetParticleElementCollision(): ...
