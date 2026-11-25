# module: Texture2DInfo

import sys

CompressionNoAlpha: int = 0x6D46046000
DefaultColor: int = 0x10
Height: int = 0xE
IsDynamicRange: int = 0x6D46046000
MipGenPreset: int = 0xA
NoneCompression: int = 0x6D46046000
NoneMipDowngrading: int = 0x6D46046000
TextureType: int = 0xB
Width: int = 0xC

def GetTexture2DInfo(): ...
