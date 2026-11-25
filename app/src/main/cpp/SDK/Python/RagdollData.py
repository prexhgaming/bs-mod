# module: RagdollData

import sys

Constraints: int = 0x38
DisableCollisionPairs: int = 0x80
HighToLowMapper: int = 0x108
LowToHighMapper: int = 0x98
Parents: int = 0x20
RBToBone: int = 0x50
RBToBoneTrans: int = 0x68

def GetRagdollData(): ...
