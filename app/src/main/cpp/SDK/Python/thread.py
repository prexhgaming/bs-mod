# module: _thread

import sys

RLock: int = 0x4500
local: int = 0x4500
localdummy: int = 0x180
lock: int = 0x4180

def Getthread(): ...
