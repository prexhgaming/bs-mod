# module: IArea

import sys

AreaId: int = 0x60
ConnectedAreas: int = 0xD8
Entities: int = 0xB0
IsFocusing: int = 0x66
IsInWorld: int = 0x65
IsIndoor: int = 0x64
IsRootArea: int = 0x67
Level: int = 0xA0
Space: int = 0xA8
Storyboard: int = 0x100
Title: int = 0x108
Transform: int = 0x68
World: int = 0x98

def GetIArea(): ...
