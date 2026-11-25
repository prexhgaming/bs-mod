# module: CageConfigData

import sys

Brakes: int = 0x34
Differential: int = 0xC
EngineInertia: int = 0x30
GearRatios: int = 0x18
Neutral: int = 0x14
Reverse: int = 0x10
TorqueCurve: int = 0x38

def GetCageConfigData(): ...
