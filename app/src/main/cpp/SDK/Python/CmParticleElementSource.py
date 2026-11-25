# module: CmParticleElementSource

import sys

ActiveDuration: int = 0x68
EmissionBurst: int = 0x60
EmissionDelay: int = 0x5C
EmissionDuration: int = 0x58
EmissionRate: int = 0x50
EmissionRateByDistance: int = 0x54
SleepDuration: int = 0x6C
TimeTriggered: int = 0x64

def GetCmParticleElementSource(): ...
