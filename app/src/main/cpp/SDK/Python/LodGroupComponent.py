# module: LodGroupComponent

import sys

LodStartLevel: int = 0x2C0
OverrideLodDistance: int = 0x2A8
OverrideTransitionDistance: int = 0x2A0
SubModels: int = 0x288

def GetLodGroupComponent(): ...
