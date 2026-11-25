# module: PhysicsRigidBodyTemplateData

import sys

CollisionProperties: int = 0x2C
DynamicProperties: int = 0x50
MaterialGuids: int = 0x70
MotionType: int = 0x68
Shapes: int = 0x88

def GetPhysicsRigidBodyTemplateData(): ...
