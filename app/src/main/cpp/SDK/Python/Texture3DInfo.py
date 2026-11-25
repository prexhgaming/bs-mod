# module: Texture3DInfo

import sys

CompressionNoAlpha: int = 0x6D46046000
DefaultColor: int = 0x14
Depth: int = 0x12
Height: int = 0x10
IsDynamicRange: int = 0x6D46046000
IsUINT: int = 0x6D46046000
LODGroup: int = 0xA
MipGenPreset: int = 0xB
NoneCompression: int = 0x6D46046000
NoneMipDowngrading: int = 0x6D46046000
Width: int = 0xE
padding: int = 0x6D46046000

def GetTexture3DInfo(): ...
