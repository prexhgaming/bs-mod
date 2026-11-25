# module: ClientManipulator

import sys

ActiveGizmo: int = 0x2B0
Camera: int = 0x220
GizmoSize: int = 0x2B8
Gizmos: int = 0x288
SubviewManager: int = 0x280
Target: int = 0x228
TargetTransform: int = 0x250
World: int = 0x218

def GetClientManipulator(): ...
