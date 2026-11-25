# module: VehicleFlyPlatformTemplate

import sys

EulerAngleControlDampingRatio: int = 0x30
EulerAngleControlFrequence: int = 0x40
EulerAngleStableDamperThreshold: int = 0x38
EulerAngleStableDampingRatio: int = 0x2C
EulerAngleStableFrequence: int = 0x3C
EulerAngleStableStrength: int = 0x34
MaxEulerAngAcc: int = 0x28
MaxSideAcc: int = 0x20
MaxSteerSpeed: int = 0x24
PitchRange: int = 0x48
RBGuid: int = 0x10
SideDampRatio: int = 0x44
SteerRoll: int = 0x4C

def GetVehicleFlyPlatformTemplate(): ...
