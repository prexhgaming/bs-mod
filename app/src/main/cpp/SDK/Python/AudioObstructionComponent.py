# module: AudioObstructionComponent

import sys

EnableDiffraction: int = 0x228
EnableDiffractionOnBoundary: int = 0x229
GeometryID: int = 0x208
Occlusion: int = 0x22C
Room: int = 0x210
Sound: int = 0x200
Texture: int = 0x230

def GetAudioObstructionComponent(): ...
