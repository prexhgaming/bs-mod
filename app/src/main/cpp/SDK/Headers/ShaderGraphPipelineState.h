class ShaderGraphPipelineState {
  uintptr_t BlendColorWriteMask; // Offset: 0x12
  uintptr_t BlendDst; // Offset: 0x16
  uintptr_t BlendDstAlpha; // Offset: 0x18
  uintptr_t BlendOp; // Offset: 0x13
  uintptr_t BlendOpAlpha; // Offset: 0x14
  uintptr_t BlendSrc; // Offset: 0x15
  uintptr_t BlendSrcAlpha; // Offset: 0x17
  uintptr_t StencilDepthFailOp; // Offset: 0xB
  uintptr_t StencilDepthFailOpBack; // Offset: 0xF
  uintptr_t StencilFailOp; // Offset: 0xA
  uintptr_t StencilFailOpBack; // Offset: 0xE
  uintptr_t StencilFunc; // Offset: 0xD
  uintptr_t StencilFuncBack; // Offset: 0x11
  uintptr_t StencilPassOp; // Offset: 0xC
  uintptr_t StencilPassOpBack; // Offset: 0x10
};
