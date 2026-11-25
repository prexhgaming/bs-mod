# module: FlamencoHairInterpolationData

import sys

DriveCurveIndices: int = 0x58
DriveGlobalIndices: int = 0x70
GuideCurveIndices: int = 0x40
GuideVertexWeights: int = 0xA0
TangentDirectionLerps: int = 0x88

def GetFlamencoHairInterpolationData(): ...
