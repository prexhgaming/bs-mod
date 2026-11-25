# module: VehicleOrientationAssistorParamsWrapper

import sys

PitchRotDelayTime: int = 0x6D46046000
PitchRotEnabled: int = 0x6D46046000
PitchRotFallRate: int = 0x6D46046000
PitchRotInput: int = 0x6D46046000
PitchRotInputByAccel: int = 0x6D46046000
PitchRotMaxAcc: int = 0x6D46046000
PitchRotRiseRate: int = 0x6D46046000
PitchRotSpeed: int = 0x6D46046000
YawRotDelayTime: int = 0x6D46046000
YawRotEnabled: int = 0x6D46046000
YawRotFallRate: int = 0x6D46046000
YawRotInput: int = 0x6D46046000
YawRotInputBySteer: int = 0x6D46046000
YawRotMaxAcc: int = 0x6D46046000
YawRotRiseRate: int = 0x6D46046000
YawRotSpeed: int = 0x6D46046000

def GetVehicleOrientationAssistorParamsWrapper(): ...
