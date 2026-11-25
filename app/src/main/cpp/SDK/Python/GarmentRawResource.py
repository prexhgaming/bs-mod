# module: GarmentRawResource

import sys

ColShapes: int = 0xE0
GarmentMesh: int = 0x80
GarmentPieces: int = 0x38
GarmentRenderInfo: int = 0x140
GarmentSeamLines: int = 0x68
MaxDistance: int = 0xC8
PhysicsProperties: int = 0x50

def GetGarmentRawResource(): ...
