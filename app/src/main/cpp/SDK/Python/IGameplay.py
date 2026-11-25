# module: IGameplay

import sys

Controller: int = 0x50
HoldingInstance: int = 0x68
IsSimulated: int = 0x4D
IsStarted: int = 0x4C
LoadingShelter: int = 0x80
Player: int = 0x58
Scenario: int = 0x60

def GetIGameplay(): ...
