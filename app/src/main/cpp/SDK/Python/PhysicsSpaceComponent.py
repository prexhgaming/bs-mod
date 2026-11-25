# module: PhysicsSpaceComponent

import sys

ChunkEdgeLength: int = 0x20C
ChunkHalfHeight: int = 0x210
CurrentFocusCenter: int = 0x1F8
DefaultFocusCenter: int = 0x1EC
Displayer: int = 0x220
EnablePhysicsLoading: int = 0x6D46046000
EnableUpdateFocusCenter: int = 0x208
FocusHalfChunkSize: int = 0x214
FocusSource: int = 0x204
ForceLoadAllRigidBodies: int = 0x228
MaxTickPerFrame: int = 0x1E8
PvdIpToConnect: int = 0x230
ReconnectToPvd: int = 0x248
WorldHalfSize: int = 0x218

def GetPhysicsSpaceComponent(): ...
