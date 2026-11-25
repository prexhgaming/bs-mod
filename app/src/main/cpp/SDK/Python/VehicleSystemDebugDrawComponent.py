# module: VehicleSystemDebugDrawComponent

import sys

CMLocalFrameScale: int = 0x84
FrameScale: int = 0x80
ShowCMLocalFrame: int = 0x8E
ShowFrame: int = 0x8D
ShowSuspOrigData: int = 0x8B
ShowSuspensions: int = 0x8A
ShowTrailerLinks: int = 0x8C
ShowVehicleRB: int = 0x88
ShowVehicleRBInterpolatedPose: int = 0x89
SuspensionsColor: int = 0x70
VehicleRBColor: int = 0x60

def GetVehicleSystemDebugDrawComponent(): ...
