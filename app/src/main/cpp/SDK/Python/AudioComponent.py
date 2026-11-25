# module: AudioComponent

import sys

AudioShape: int = 0x1F9
AudioType: int = 0x1F8
ControlPoints: int = 0x200
Direction: int = 0x228
EventID: int = 0x198
EventParam: int = 0x1C8
ExtraEventIDs: int = 0x1A8
ExtraEventParams: int = 0x1E0
FadeOutTime: int = 0x21C
FadeOutType: int = 0x220
FollowListener: int = 0x222
GameObjectID: int = 0x1A0
IsDynamic: int = 0x223
IsInVolume: int = 0x160
IsSpatial: int = 0x224
LeaveWithoutStop: int = 0x221
LocalBound: int = 0x164
OneShot: int = 0x225
PolygonRampDistance: int = 0x218
Sound: int = 0x158
UpToOne: int = 0x226
Version: int = 0x1C0
WorldBound: int = 0x17C

def GetAudioComponent(): ...
