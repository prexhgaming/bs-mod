# module: TimeOfDayPropTrack

import sys

IsNew: int = 0x88
KeyFrameData: int = 0x30
KeyFrameStride: int = 0x8C
KeyFrameTime: int = 0x18
PropWidths: int = 0x48
SingleKeyFrames: int = 0x60

def GetTimeOfDayPropTrack(): ...
