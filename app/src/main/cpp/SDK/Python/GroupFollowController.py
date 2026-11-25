# module: GroupFollowController

import sys

Aspect: int = 0xE0
DistanceRange: int = 0xD8
FieldOfView: int = 0xC0
MaxArea: int = 0xCC
MinArea: int = 0xC4
Pitch: int = 0xB4
Position: int = 0xA8
Roll: int = 0xBC
Speed: int = 0xD4
Targets: int = 0x90
Yaw: int = 0xB8

def GetGroupFollowController(): ...
