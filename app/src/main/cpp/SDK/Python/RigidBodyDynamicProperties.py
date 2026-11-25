# module: RigidBodyDynamicProperties

import sys

DisableGravity: int = 0x14
EnableCCD: int = 0x15
MaxAngularSpeed: int = 0xC
SleepThreshold: int = 0x10

def GetRigidBodyDynamicProperties(): ...
