# module: ConstraintDataBase

import sys

Actor0Id: int = 0x10
Actor1Id: int = 0x28
BreakForceThreshold: int = 0xAC
BreakTorqueThreshold: int = 0xB0
EnableProjection: int = 0xA0
IsBreakable: int = 0xB4
ProjectionAngularTolerance: int = 0xA8
ProjectionLinearTolerance: int = 0xA4
Trans0: int = 0x40
Trans1: int = 0x70

def GetConstraintDataBase(): ...
