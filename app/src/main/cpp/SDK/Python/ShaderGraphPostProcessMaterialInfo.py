# module: ShaderGraphPostProcessMaterialInfo

import sys

BlendableLocation: int = 0x5C
BlendablePriority: int = 0x58
CurveParameters: int = 0x30
OverrideLocation: int = 0x98
OverrideParameters: int = 0x80
OverridePipelineState: int = 0x99
Parameters: int = 0x18
PipelineState: int = 0x5D
VaringMask: int = 0x78

def GetShaderGraphPostProcessMaterialInfo(): ...
