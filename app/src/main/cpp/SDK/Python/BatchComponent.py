# module: BatchComponent

import sys

BatchKey: int = 0x35C
InstanceBound: int = 0x360
InstanceOccluIds: int = 0x300
Instances: int = 0x2E8
IsLightmapReady: int = 0x359
LightmapAlwaysLoad: int = 0x358
LightmapID: int = 0x318
LightmapOffsets: int = 0x328
LightmapScales: int = 0x340
ReceiveDecals: int = 0x35A

def GetBatchComponent(): ...
