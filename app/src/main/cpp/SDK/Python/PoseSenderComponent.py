# module: PoseSenderComponent

import sys

AccTime: int = 0xE0
DebugOutput: int = 0xA0
DebugOutput2: int = 0xA1
EnableSend: int = 0x91
EnableSendGraphState: int = 0x90
GraphSendDelta: int = 0x84
PlatformEntity: int = 0xA8
PredictFadeIn: int = 0x98
PredictFadeOut: int = 0x9C
PredictMoveDeltaTime: int = 0x94
SendInterval: int = 0x80
ThresholdSpeed: int = 0x88
ThresholdYawSpeed: int = 0x8C

def GetPoseSenderComponent(): ...
