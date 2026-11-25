# module: VehicleOrientationAssistorData

import sys

PitchRotDelayTime: int = 0x28
PitchRotEnabled: int = 0x30
PitchRotFallRate: int = 0x20
PitchRotInput: int = 0x10
PitchRotInputByAccel: int = 0x32
PitchRotRiseRate: int = 0x18
YawRotDelayTime: int = 0x2C
YawRotEnabled: int = 0x31
YawRotFallRate: int = 0x24
YawRotInput: int = 0x14
YawRotInputBySteer: int = 0x33
YawRotMaxAcc: int = 0xC
YawRotRiseRate: int = 0x1C

def GetVehicleOrientationAssistorData(): ...
