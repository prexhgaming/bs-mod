# module: TriangleMeshShapeData

import sys

Indices: int = 0x28
IsDeformable: int = 0x58
MaterialIDs: int = 0x40
Vertices: int = 0x10

def GetTriangleMeshShapeData(): ...
