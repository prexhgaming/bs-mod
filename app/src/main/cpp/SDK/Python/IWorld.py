# module: IWorld

import sys

ActiveLevels: int = 0x118
Anchor: int = 0x1E8
AppearableReadyCount: int = 0x1F8
AppearableTotalCount: int = 0x1F4
DefaultLevel: int = 0x110
EnvVolume: int = 0x1A8
FilePath: int = 0x200
Gameplay: int = 0x108
InterestPoint: int = 0x148
IsInitialized: int = 0xDC
IsLockingAudio: int = 0xDF
IsLockingProxy: int = 0xDE
IsReadyToAppear: int = 0xDD
Levels: int = 0xE0
NavigateMap: int = 0x1C8
ObservationPoints: int = 0x188
PSVolumeSpace: int = 0x1C0
PhysicsSpace: int = 0x1B0
PreloadDistances: int = 0x170
PreloadPivots: int = 0x158
PvsSpace: int = 0x1B8
Scene: int = 0x140
Storyboard: int = 0x1A0
SubNavigateMaps: int = 0x1D0
Title: int = 0xC0
Version: int = 0xD8

def GetIWorld(): ...
