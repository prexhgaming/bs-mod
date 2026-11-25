# module: ShaderGraphInfo

import sys

BlendableLocation: int = 0x74
BlendablePriority: int = 0x70
BranchTreeData: int = 0x98
CurveParameters: int = 0x48
CustomMask: int = 0x118
MaterialProperties: int = 0xB0
ParameterCollections: int = 0xF0
ParameterEmbeddedName: int = 0xC8
PipelineState: int = 0x75
VGCustomProperties: int = 0x148
VGProperties: int = 0x120
VaringMask: int = 0x90

def GetShaderGraphInfo(): ...
