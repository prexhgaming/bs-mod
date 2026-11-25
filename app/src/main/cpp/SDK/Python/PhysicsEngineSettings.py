# module: PhysicsEngineSettings

import sys

DMaxDamageThreshold: int = 0x10
DMinDamageThreshold: int = 0xC
DynamicFriction: int = 0x18
Restitution: int = 0x1C
StaticFriction: int = 0x14

def GetPhysicsEngineSettings(): ...
