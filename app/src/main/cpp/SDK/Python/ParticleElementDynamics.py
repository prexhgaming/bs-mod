# module: ParticleElementDynamics

import sys

Attractor: int = 0x60
AttractorAttenuation: int = 0x7C
AttractorScale: int = 0x6C
AttractorStrength: int = 0x78
DragCoefficient: int = 0x5C
Gravity: int = 0x50

def GetParticleElementDynamics(): ...
