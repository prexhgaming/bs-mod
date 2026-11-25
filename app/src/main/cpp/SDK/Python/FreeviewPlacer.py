# module: FreeviewPlacer

import sys

AffiliatedFov: int = 0xDC
Direction: int = 0xAC
DirectionHalflife: int = 0xC4
DirectionTolerance: int = 0xC8
Finished: int = 0x88
FocusHalflife: int = 0xA4
FocusPos: int = 0x8C
FocusTolerance: int = 0xA8
Fov: int = 0xCC
FovHalflife: int = 0xD4
FovTolerance: int = 0xD8
PrevDirection: int = 0xB8
PrevFocusPos: int = 0x98
PrevFov: int = 0xD0

def GetFreeviewPlacer(): ...
