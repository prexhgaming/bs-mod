# module: TwParticleElementSource

import sys

ActiveDuration: int = 0x64
EmissionBurst: int = 0x5C
EmissionDelay: int = 0x58
EmissionDuration: int = 0x54
EmissionRate: int = 0x50
SleepDuration: int = 0x68
TimeTriggered: int = 0x60

def GetTwParticleElementSource(): ...
