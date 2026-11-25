# module: IPrimitiveComponent

import sys

CustomDepthStencilValue: int = 0x1BA
CustomRenderSet: int = 0x1B0
CustomShadowBias: int = 0x1C0
CustomStencil: int = 0x1B8
InheritClipFlag: int = 0x1E0
InheritTwoSidedFlag: int = 0x1E1
IsCastDynamicShadow: int = 0x1BB
IsIgnoreInheritFlag: int = 0x1E8
IsInSpace: int = 0x1E4
IsPointCloudLight: int = 0x1E7
IsReadyToAppear: int = 0x1E3
IsReceiveDynamicShadow: int = 0x1BC
IsResourceReady: int = 0x1E2
IsVisible: int = 0x1E5
IsWarmingUp: int = 0x1E6
LocalBound: int = 0x150
OccluId: int = 0x1C8
OcclusFlag: int = 0x1C5
OverrideMaterial: int = 0x1D0
PrimLocalBound: int = 0x180
PrimWorldBound: int = 0x198
RenderInCustomDepthPass: int = 0x1B9
Resource: int = 0x140
WorldBound: int = 0x168

def GetIPrimitiveComponent(): ...
