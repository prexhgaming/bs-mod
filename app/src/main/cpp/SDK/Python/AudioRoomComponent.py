# module: AudioRoomComponent

import sys

AuxSend: int = 0x230
EnableDiffraction: int = 0x235
EnableDiffractionOnBoundary: int = 0x236
EnableObstruction: int = 0x234
GeometryID: int = 0x200
Index: int = 0x1E8
Occlusion: int = 0x238
Priority: int = 0x208
ReverbAuxBus: int = 0x210
ReverbLevel: int = 0x228
RoomID: int = 0x1F8
Sound: int = 0x1F0
Texture: int = 0x240
WallOcclusion: int = 0x22C

def GetAudioRoomComponent(): ...
