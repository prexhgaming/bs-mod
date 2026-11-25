# module: ConstraintConfig

import sys

BreakForceThreshold: int = 0x6D46046000
BreakTorqueThreshold: int = 0x6D46046000
ConstraintType: int = 0x6D46046000
EnableProjection: int = 0x6D46046000
IsBreakable: int = 0x6D46046000
ProjectionAngularTolerance: int = 0x6D46046000
ProjectionLinearTolerance: int = 0x6D46046000
RevoluteConfig: int = 0x50
Trans0: int = 0x6D46046000
Trans1: int = 0x6D46046000

def GetConstraintConfig(): ...
