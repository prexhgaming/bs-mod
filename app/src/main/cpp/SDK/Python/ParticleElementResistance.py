# module: ParticleElementResistance

import sys

DelayApply: int = 0x5C
ResistanceConstant: int = 0x50
ResistanceLinear: int = 0x54
ResistanceQuadratic: int = 0x58

def GetParticleElementResistance(): ...
