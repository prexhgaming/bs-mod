# module: VisualGraphExtBufferData

import sys

PrimitiveIndex: int = 0x40
SlotMask: int = 0x3C
Stride: int = 0x38
TextureArrayDepth: int = 0x41
TextureArrayLayout: int = 0x48
Type: int = 0x18
Value: int = 0x20

def GetVisualGraphExtBufferData(): ...
