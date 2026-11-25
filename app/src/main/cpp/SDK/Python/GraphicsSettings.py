# module: GraphicsSettings

import sys

BillboardDistance: int = 0x54
CSM_Distance0: int = 0x1C
CSM_Distance1: int = 0x20
CSM_Distance2: int = 0x24
CSM_Distance3: int = 0x28
CSM_FilterRange0: int = 0x2C
CSM_FilterRange1: int = 0x30
CSM_FilterRange2: int = 0x34
CSM_FilterRange3: int = 0x38
CSM_Layers: int = 0x3C
CandelaProbeSH3: int = 0xB4
CandelaTransparencyGI: int = 0xB5
DeferredLighting: int = 0xA
EnableClipPrePass: int = 0x83
EnableColorGrading: int = 0xC
EnableCustomDepthStencil: int = 0x8C
EnableFoliageIndirect: int = 0x95
EnableGPUCloth: int = 0x94
EnableGPUParticle: int = 0x8E
EnableOcclus: int = 0x8D
EnableReport: int = 0x80
EnableSkinCache: int = 0xB0
EnableTessellation: int = 0xB6
EnableVisualGraph: int = 0x92
FontEffect: int = 0x81
FontVerticalSpacing: int = 0x88
ForwardCSM: int = 0xB3
GPUParticleBufferSize: int = 0x90
HighDynamicRangeRender: int = 0xB
IsRoughnessLevelPow2: int = 0xB2
LodDistance1: int = 0x40
LodDistance2: int = 0x44
LodDistance3: int = 0x48
LodMeshFadeTime: int = 0x60
MaxDetailLevel: int = 0x7E
MaxShaderLodLevel: int = 0xF
MinShaderLodLevel: int = 0xE
MipFocusDistance: int = 0x58
MipMinLevel: int = 0x7F
MipResistDistance: int = 0x5C
MipUpdateFrames: int = 0x7C
NearPlane: int = 0x84
PlanarShadowFilterInfo: int = 0x64
PlanarShadowTMax: int = 0x70
PlanarShadowTraceDistance: int = 0x6C
SDFVolumeSize0: int = 0x4C
SDFVolumeSize1: int = 0x50
SceneUIFilterInfo: int = 0x68
ScreenScale: int = 0x18
ShaderCacheDirectory: int = 0x98
ShaderMacros: int = 0xB8
Shaders: int = 0xD0
ShadowMapSize: int = 0x10
ShadowYClip: int = 0x14
SunLightShadow: int = 0xD
SunShadowDistanceFar: int = 0x78
SunShadowDistanceNear: int = 0x74
SupportMipDistance: int = 0x82
UseOctahedronMap: int = 0xB1
VisualGraphUseTextureBuffer: int = 0x93

def GetGraphicsSettings(): ...
