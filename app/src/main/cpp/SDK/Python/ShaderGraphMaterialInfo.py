# module: ShaderGraphMaterialInfo

import sys

CurveParameters: int = 0x48
DepthOffset: int = 0xC
MaterialFlags: int = 0x10
ParameterCollections: int = 0x70
Parameters: int = 0x30
ShaderName: int = 0x18

def GetShaderGraphMaterialInfo(): ...
