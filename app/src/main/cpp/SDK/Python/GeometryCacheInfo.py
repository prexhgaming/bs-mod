# module: GeometryCacheInfo

import sys

EndFrameTime: int = 0x10
GeometryCacheTree: int = 0x20
IsConstantTopology: int = 0x18
SecondsPerFrame: int = 0x14
StartFrameTime: int = 0xC

def GetGeometryCacheInfo(): ...
