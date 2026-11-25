# module: ParticleElementTurbulence

import sys

PhaseDistribution: int = 0x60
SpaceFreqency: int = 0x54
TimeFreqency: int = 0x58
Turbulence: int = 0x50

def GetParticleElementTurbulence(): ...
