# module: SceneGuiderComponent

import sys

BlendTime: int = 0xD4
CachedEntity: int = 0x120
CameraPriority: int = 0x114
CameraTransform: int = 0xE4
Duration: int = 0xD8
EpisodeIndex: int = 0xD0
FilePath: int = 0xB8
FiniRatio: int = 0xE0
GuidingType: int = 0xB0
InitRatio: int = 0xDC
VirtualCamera: int = 0x118

def GetSceneGuiderComponent(): ...
