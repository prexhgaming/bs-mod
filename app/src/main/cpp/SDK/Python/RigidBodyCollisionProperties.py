# module: RigidBodyCollisionProperties

import sys

QueryFilterData0: int = 0x10
QueryFilterData1: int = 0x14
QueryFilterData2: int = 0x18
QueryFilterData3: int = 0x1C
RBShapeFlags: int = 0x20
SimFilterData3: int = 0xC

def GetRigidBodyCollisionProperties(): ...
