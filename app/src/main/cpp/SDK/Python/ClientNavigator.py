# module: ClientNavigator

import sys

Camera: int = 0x1F8
CameraController: int = 0x200
MoveSpeed: int = 0x208

def GetClientNavigator(): ...
