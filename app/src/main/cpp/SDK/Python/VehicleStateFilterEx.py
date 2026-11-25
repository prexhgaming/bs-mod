# module: VehicleStateFilterEx

import sys

DebugOutput: int = 0x9D
EnableSend: int = 0x8C
EnableSendSnapshot: int = 0x8D
InterpMethod: int = 0x94
LocalMode: int = 0x80
MaxBlendTime: int = 0x98
SendInterval: int = 0x84
SendSnapshotInterval: int = 0x88
SnapshotVersion: int = 0x90
SyncSnapshot: int = 0x9C
UseClientPhysxVehicleSimulate: int = 0x9E

def GetVehicleStateFilterEx(): ...
