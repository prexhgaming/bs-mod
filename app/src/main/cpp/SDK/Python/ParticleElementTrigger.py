# module: ParticleElementTrigger

import sys

Event: int = 0x50
EventName: int = 0x58
InheritColor: int = 0x79
InheritLife: int = 0x7A
InheritRotation: int = 0x7C
InheritSize: int = 0x7B
InheritVelocity: int = 0x7D
InheritVelocityScale: int = 0x80
KeepActive: int = 0x78
SpawnCount: int = 0x70

def GetParticleElementTrigger(): ...
