# module: INavigateObstacle

import sys

AreaID: int = 0xC8
AutoEnterMainMap: int = 0xCC
Enable: int = 0x68
NaviMap: int = 0xD0
OffsetTransform: int = 0x98
ShapePath: int = 0x70
ShapeRes: int = 0x88

def GetINavigateObstacle(): ...
