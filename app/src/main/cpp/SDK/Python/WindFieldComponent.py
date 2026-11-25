# module: WindFieldComponent

import sys

AdvectionBlendParam: int = 0x314
AmbientOnly: int = 0x324
AmbientWind: int = 0x2A4
DebugDrawer: int = 0x298
DeltaTime: int = 0x30C
DiffusionRate: int = 0x310
EnableAdvection: int = 0x320
EnableDiffusion: int = 0x321
EnableNoiseAmbient: int = 0x323
EnableProjection: int = 0x322
FieldSize: int = 0x2E0
FollowEntity: int = 0x2A1
GridLength: int = 0x2EC
Is3D: int = 0x325
NBDiffusionIter: int = 0x318
NBProjectIter: int = 0x31C
NoiseDisturbance: int = 0x2D8
NoiseTexId: int = 0x2B0
NoiseTexToWorld: int = 0x2C0
NoiseUVResetTime: int = 0x2DC
NoiseUVSize: int = 0x2CC
PositionOffset: int = 0x2F0

def GetWindFieldComponent(): ...
