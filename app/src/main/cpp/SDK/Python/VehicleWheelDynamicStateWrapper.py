# module: VehicleWheelDynamicStateWrapper

import sys

CamberAngle: int = 0x98
IsInAir: int = 0x40
LateralSlip: int = 0x90
LocalPose: int = 0x9C
LongitudinalSlip: int = 0x8C
SteerAngle: int = 0x94
SuspJounce: int = 0x6C
SuspSpringForce: int = 0x70
TireContactNormal: int = 0x5C
TireContactPoint: int = 0x50
TireFriction: int = 0x68
TireLateralDir: int = 0x80
TireLongitudinalDir: int = 0x74
WheelRotationAngle: int = 0xBC
WheelSpeed: int = 0xB8

def GetVehicleWheelDynamicStateWrapper(): ...
