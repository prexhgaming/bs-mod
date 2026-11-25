# module: SceneNodeComponent

import sys

AlphaScale: int = 0xA8
BorderFlag: int = 0xB4
FovDistance: int = 0xA0
MaxOpacity: int = 0xB0
MaxScale: int = 0x9C
MinOpacity: int = 0xAC
MinScale: int = 0x98
VisibleDistance: int = 0xA4

def GetSceneNodeComponent(): ...
