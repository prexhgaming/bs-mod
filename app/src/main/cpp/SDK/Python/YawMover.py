# module: YawMover

import sys

AngularAccelerateTime: int = 0x174
AngularAcceleration: int = 0x170
AngularRatio: int = 0x18C
AngularSpeed: int = 0x17C
AngularTolerance: int = 0x19C
BrakeAngle: int = 0x178
Clockwise: int = 0x16D
EscapedTime: int = 0x160
FocusRatio: int = 0x190
FocusSpeed: int = 0x180
FocusTolerance: int = 0x1A0
FovRatio: int = 0x164
FovTolerance: int = 0x168
LerpAsBrake: int = 0x16C
RadialRatio: int = 0x194
RadialSpeed: int = 0x184
RadialTolerance: int = 0x1A4
VerticalRatio: int = 0x198
VerticalSpeed: int = 0x188
VerticalTolerance: int = 0x1A8

def GetYawMover(): ...
