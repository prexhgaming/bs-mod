# module: RigidBodyMassProperties

import sys

Mass: int = 0x28
MassLocalPoseRot: int = 0xC
MassSpaceInertiaTensor: int = 0x1C

def GetRigidBodyMassProperties(): ...
