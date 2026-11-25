# module: VisualGraphTaskNode

import sys

CurveParameters: int = 0x80
Depth: int = 0x16
DstBuffer: int = 0x14
ExposedParameterIndices: int = 0x68
MaxTriggerNum: int = 0x1C
Parameters: int = 0x50
PostDepth: int = 0x18
PropertyParameters: int = 0xA8
ShaderExt: int = 0x20
SrcBuffer: int = 0x12
Technique: int = 0x38
Type: int = 0x10

def GetVisualGraphTaskNode(): ...
