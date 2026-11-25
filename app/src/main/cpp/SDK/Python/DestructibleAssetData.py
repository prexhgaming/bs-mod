# module: DestructibleAssetData

import sys

CollisionProperties: int = 0x28
DynamicProperties: int = 0x4C
PhysicsMaterial: int = 0x18

def GetDestructibleAssetData(): ...
