# module: ActionComponent

import sys

Channel: int = 0x8C
Delay: int = 0x84
EnterWorldPlay: int = 0x8F
Identifier: int = 0x68
IsPlaying: int = 0x90
RemoveMotion: int = 0x8E
SinglePlay: int = 0x8D
Speed: int = 0x80
StartTime: int = 0x88

def GetActionComponent(): ...
