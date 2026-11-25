# module: SoundComponent

import sys

ClampDistanceMap: int = 0x110
DeclinePerEvent: int = 0x100
DeclinePerMeter: int = 0x108
DistanceLimit: int = 0x104
EnableClamp: int = 0xF8
EventLimitCount: int = 0xFC
ListenerPositionBias: int = 0xF4
ListenerRole: int = 0x138
RolePositionBias: int = 0xE8
Rooms: int = 0x140

def GetSoundComponent(): ...
