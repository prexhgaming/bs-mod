# module: ParticleElementInheritVelocity

import sys

FollowAcceleration: int = 0x68
FollowMode: int = 0x64
InheritDuration: int = 0x5C
InheritScale: int = 0x50
MaxVelocity: int = 0x60

def GetParticleElementInheritVelocity(): ...
