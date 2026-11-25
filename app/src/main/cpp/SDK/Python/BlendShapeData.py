# module: BlendShapeData

import sys

BlendShapeIndices: int = 0x20
ChannelName: int = 0x50
Deformation: int = 0x68
RenderMappedIndices: int = 0x38

def GetBlendShapeData(): ...
