# module: VirtualCameraManager

import sys

BlendStack: int = 0x3F8
ControlledCamera: int = 0x2F8
Controllers: int = 0x3E0
Debugger: int = 0x3D8
Enable: int = 0x3D0
ModifyStack: int = 0x410

def GetVirtualCameraManager(): ...
