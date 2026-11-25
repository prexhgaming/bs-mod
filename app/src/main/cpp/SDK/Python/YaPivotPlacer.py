# module: YaPivotPlacer

import sys

Aspect: int = 0xD4
Direction: int = 0xF8
Finished: int = 0xC0
FocusPosOffset: int = 0xE0
FocusRatio: int = 0xB8
Fov: int = 0xD0
MajorTarget: int = 0x138
MajorTargetPos: int = 0x11C
MajorTargetPosOffset: int = 0x104
MajorTargetTransform: int = 0x148
MarginPercent: int = 0xC4
MaxDist: int = 0xCC
MinDist: int = 0xC8
MinorTarget: int = 0x140
MinorTargetPos: int = 0x128
MinorTargetPosOffset: int = 0x110
MinorTargetTransform: int = 0x178
PitchAngle: int = 0xD8
PivotRatio: int = 0xDC
RadialRatio: int = 0xBC
RangeMaxPoint: int = 0xAC
RangeMinPoint: int = 0xA0
ReferDirection: int = 0xEC

def GetYaPivotPlacer(): ...
