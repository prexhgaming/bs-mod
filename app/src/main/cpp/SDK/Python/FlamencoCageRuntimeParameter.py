# module: FlamencoCageRuntimeParameter

import sys

Accel: int = 0x40
Brake: int = 0x41
Reverse: int = 0x42
SteerLeft: int = 0x43
SteerRight: int = 0x44
SystemReporter: int = 0x58
SystemScaler: int = 0x48

def GetFlamencoCageRuntimeParameter(): ...
