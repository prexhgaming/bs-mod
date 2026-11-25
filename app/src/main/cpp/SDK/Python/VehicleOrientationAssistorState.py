# module: VehicleOrientationAssistorState

import sys

PitchRotActivated: int = 0x28
PitchRotDelayedTime: int = 0x20
YawRotActivated: int = 0x29
YawRotDelayedTime: int = 0x24
YawRotTorqueAcc: int = 0x14

def GetVehicleOrientationAssistorState(): ...
