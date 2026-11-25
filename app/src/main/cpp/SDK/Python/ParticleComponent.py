# module: ParticleComponent

import sys

AudioSwitches: int = 0x3A0
CameraAttaching: int = 0x2FC
DeferredTick: int = 0x312
HoldResource: int = 0x311
InstanceColor: int = 0x2EC
IsActive: int = 0x30E
IsPaused: int = 0x30F
IsStopped: int = 0x310
LODGroup: int = 0x304
LodDistance: int = 0x308
MiscFlags: int = 0x313
ParticleParameters: int = 0x2C0
RelativeVolume: int = 0x300
SleepPeriod: int = 0x2E8

def GetParticleComponent(): ...
