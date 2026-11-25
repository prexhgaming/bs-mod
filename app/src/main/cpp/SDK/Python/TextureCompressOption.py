# module: TextureCompressOption

import sys

Compression: int = 0x18
DownSampleScale: int = 0xC
Format: int = 0x10
ItemName: int = 0x50
KeepRaw: int = 0x3A
KeepRawMips: int = 0x3B
LinearSpaceResize: int = 0x3E
LosslessCompress: int = 0x3C
MaxTextureSize: int = 0x2C
MipDiyParameter: int = 0x40
MipGen: int = 0x19
MixRoughenRate: int = 0x34
NoneCompression: int = 0x3D
NormalSmoothRate: int = 0x30
Premultiply: int = 0x24
Quality: int = 0x14
ResizeFilter: int = 0x3F
RoughenMixMap: int = 0x39
SRGBIn: int = 0x1C
SRGBOut: int = 0x20
SeparateAlpha: int = 0x28
SmoothNormalMap: int = 0x38
SourceFilePath: int = 0x68

def GetTextureCompressOption(): ...
