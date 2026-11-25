# module: RecastMapDisplayerComponent

import sys

Display: int = 0xF4
HeightOffset: int = 0xF0
LayerId: int = 0xFC
QueryFlag: int = 0xF8
ShowModeFlags: int = 0x100

def GetRecastMapDisplayerComponent(): ...
