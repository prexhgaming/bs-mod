# module: RigidBodyComponent

import sys

AngularDamping: int = 0x2FC
AngularVel: int = 0x2EC
CenterOfMass: int = 0x304
ControllEntity: int = 0x358
EnableCCD: int = 0x300
EnableGravity: int = 0x31D
FollowTarget: int = 0x420
InertiaTensor: int = 0x310
IsTrigger: int = 0x31C
LinearDamping: int = 0x2F8
LinearVel: int = 0x2E0
LocalBound: int = 0x368
Mass: int = 0x354
MotionType: int = 0x350
RbTrans: int = 0x3F0
Scale: int = 0x35C
ShapePath: int = 0x3A0
ShapeRes: int = 0x3B8
WorldBound: int = 0x380

def GetRigidBodyComponent(): ...
