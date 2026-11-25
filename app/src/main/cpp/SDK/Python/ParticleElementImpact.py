# module: ParticleElementImpact

import sys

FilterInfo: int = 0x50
MaxDistanceDivergence: int = 0x5C
MaxNormalDivergence: int = 0x60
PrimaryRayDistance: int = 0x54
SecondaryRayDistance: int = 0x58
SimulateGravity: int = 0x64
UncollidedKillLife: int = 0x68

def GetParticleElementImpact(): ...
