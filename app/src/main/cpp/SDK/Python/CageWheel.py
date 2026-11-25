# module: CageWheel

import sys

ArmNode: int = 0x24
AxisNode0: int = 0x10
AxisNode1: int = 0x14
Braked: int = 0x1C
Friction: int = 0x58
Mass: int = 0x28
MaxBrokenStress: int = 0x54
MaxCompressStress: int = 0x48
MaxExpansionStress: int = 0x4C
MaxStress: int = 0x44
NearAttachNode: int = 0x38
Nodes: int = 0x60
NumRays: int = 0xC
PlasticCoef: int = 0x50
Propulsed: int = 0x20
RimDamping: int = 0x40
RimSpring: int = 0x3C
SNode: int = 0x18
Side: int = 0x34
TireDamping: int = 0x30
TireSpring: int = 0x2C

def GetCageWheel(): ...
