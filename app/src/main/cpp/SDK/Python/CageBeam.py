# module: CageBeam

import sys

D: int = 0x24
IsSupportBeam: int = 0x28
K: int = 0x20
LongBound: int = 0x30
MaxBrokenStress: int = 0x1C
MaxCompressStress: int = 0x10
MaxExpansionStress: int = 0x14
MaxStress: int = 0xC
PlasticCoef: int = 0x18
ShortBound: int = 0x2C

def GetCageBeam(): ...
