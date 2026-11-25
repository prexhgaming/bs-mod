# module: INavigateMap

import sys

AutoLoader: int = 0xA8
IsResourceReady: int = 0xD8
MapDisplayer: int = 0xB0
MapEntity: int = 0xA0
MaxAvoidNeighbours: int = 0x98
OriginOffset: int = 0x80
Resource: int = 0xC8
ShowDebug: int = 0xD9

def GetINavigateMap(): ...
