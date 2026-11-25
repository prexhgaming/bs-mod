# module: ParticleElementEvent

import sys

EventName: int = 0x50
OnBirth: int = 0x58
OnCollision: int = 0x5A
OnDeath: int = 0x59

def GetParticleElementEvent(): ...
