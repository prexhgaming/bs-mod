# module: IAppearanceComponent

import sys

BlendInOutTime: int = 0xF4
BlendRatio: int = 0xEC
DepthOffset: int = 0xF0
IsReadyToAppear: int = 0xE7
Status: int = 0xE9
Storyboard: int = 0xA8
SyncPrimitives: int = 0xE8
WorldBound: int = 0xB0

def GetIAppearanceComponent(): ...
