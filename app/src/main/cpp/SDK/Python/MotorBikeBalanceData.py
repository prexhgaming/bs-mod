# module: MotorBikeBalanceData

import sys

BalanceFailedThreshold: int = 0xC
BalanceRecoverTime: int = 0x10
EnableControll: int = 0x18
RollScale: int = 0x14

def GetMotorBikeBalanceData(): ...
