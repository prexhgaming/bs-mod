# module: ActorComponent

import sys

ApplyMotion: int = 0x1E0
GraphPath: int = 0x270
PhySpace: int = 0x2C0
PoseModifier: int = 0x288
RigidBodyAttachments: int = 0x290
TickMode: int = 0x1E4
UserTag: int = 0x1C8
UserVelocity: int = 0x1B8

def GetActorComponent(): ...
