# module: TwParticleElementJitter

import sys

AlwaysLocal: int = 0x60
JitterPeriod: int = 0x64
PositionJitter: int = 0x50
VelocityJitter: int = 0x58

def GetTwParticleElementJitter(): ...
