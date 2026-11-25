# module: PipeBlender

import sys

ApplyMotorResult: int = 0xE8
EscapedTime: int = 0xEC
Finished: int = 0xE9
Invalid: int = 0x9C
Parallel: int = 0xEA
PlacerGroup: int = 0xF8
RuntimeInfo: int = 0xB8
TotalTime: int = 0xF0

def GetPipeBlender(): ...
