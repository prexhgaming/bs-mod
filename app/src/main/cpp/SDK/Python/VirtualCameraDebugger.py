# module: VirtualCameraDebugger

import sys

ShowCamera: int = 0x80
ShowFrustum: int = 0x81
ShowName: int = 0x82

def GetVirtualCameraDebugger(): ...
