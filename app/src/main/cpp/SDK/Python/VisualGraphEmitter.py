# module: VisualGraphEmitter

import sys

AliveOffset: int = 0x56
BaseDepth: int = 0x5A
BufferCount: int = 0x58
EmitterId: int = 0x50
ExternalBufferData: int = 0x68
IsTriggered: int = 0x81
Local: int = 0x80
MaxFadeTime: int = 0x4C
MaxParticle: int = 0x48
PostBaseDepth: int = 0x5C
PostUpdateDepth: int = 0x60
PropertyStride: int = 0x54
RenderDatas: int = 0xD0
RenderTasks: int = 0xE8
SpawnIndex: int = 0x52
StartNode: int = 0xA8
TriggerSource: int = 0x88
UpdateDepth: int = 0x5E
UpdateTaskTree: int = 0xB8

def GetVisualGraphEmitter(): ...
