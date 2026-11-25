# module: ConstraintRevoluteConfig

import sys

DriveSpeed: int = 0x6D46046000
EnableDrive: int = 0x6D46046000
EnableRotLimit: int = 0x6D46046000
RotLimitLower: int = 0x6D46046000
RotLimitUpper: int = 0x6D46046000

def GetConstraintRevoluteConfig(): ...
