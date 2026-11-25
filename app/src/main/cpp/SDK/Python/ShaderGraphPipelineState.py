# module: ShaderGraphPipelineState

import sys

BlendColorWriteMask: int = 0x12
BlendDst: int = 0x16
BlendDstAlpha: int = 0x18
BlendOp: int = 0x13
BlendOpAlpha: int = 0x14
BlendSrc: int = 0x15
BlendSrcAlpha: int = 0x17
StencilDepthFailOp: int = 0xB
StencilDepthFailOpBack: int = 0xF
StencilFailOp: int = 0xA
StencilFailOpBack: int = 0xE
StencilFunc: int = 0xD
StencilFuncBack: int = 0x11
StencilPassOp: int = 0xC
StencilPassOpBack: int = 0x10

def GetShaderGraphPipelineState(): ...
