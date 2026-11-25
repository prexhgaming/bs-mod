# module: WheelSuspensionConstraintData

import sys

DriveAngularSpeed: int = 0xCC
DriveDamping: int = 0xC8
DriveSpring: int = 0xC4
EnableDrive: int = 0xD0
SuspensionDamping: int = 0xC0
SuspensionExtent: int = 0xB8
SuspensionSpring: int = 0xBC

def GetWheelSuspensionConstraintData(): ...
