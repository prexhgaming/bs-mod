# module: TachComponent

import sys

Attachments: int = 0x68
Basepoint: int = 0x98
CameraAttaching: int = 0x118
CameraSpaceTransform: int = 0x11C
EnableTachVisible: int = 0x119
Hardpoint: int = 0x80
Master: int = 0x60
Offset: int = 0xE0
RotationMode: int = 0x110
ScaleMode: int = 0x114
Transform: int = 0xB0

def GetTachComponent(): ...
