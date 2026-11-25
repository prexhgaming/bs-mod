# module: ILevel

import sys

Anchor: int = 0xC8
AppearableReadyCount: int = 0x11C
AppearableTotalCount: int = 0x118
DefaultLoad: int = 0xD6
EnableProxy: int = 0xD7
FadeOutTime: int = 0x154
FadeOutType: int = 0x158
IsInWorld: int = 0xD4
IsProxied: int = 0x164
IsReadyToAppear: int = 0xD5
Music: int = 0x120
NeedPhysics: int = 0xD8
Noise: int = 0x138
Priority: int = 0x150
ProxyDistance: int = 0x15C
ProxyResistDistance: int = 0x160
RootArea: int = 0xE0
Storyboard: int = 0x110
SubAreas: int = 0xE8
Title: int = 0x78
Transform: int = 0x98
World: int = 0x90

def GetILevel(): ...
