# module: YaFpsPlacer

import sys

BasePosition: int = 0xC4
BoneName: int = 0x90
Direction: int = 0xDC
Fov: int = 0xC0
Mover: int = 0xB0
PitchMax: int = 0xBC
PitchMin: int = 0xB8
PositionOffset: int = 0xD0
Target: int = 0xE8
UseBoneDirection: int = 0x89
UseBonePosition: int = 0x88
UseBoneRoll: int = 0x8A

def GetYaFpsPlacer(): ...
