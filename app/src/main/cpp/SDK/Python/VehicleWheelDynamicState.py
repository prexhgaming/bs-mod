# module: VehicleWheelDynamicState

import sys

CamberAngle: int = 0x4C
LateralSlip: int = 0x44
LocalPose: int = 0x50
LongitudinalSlip: int = 0x40
SteerAngle: int = 0x48
SuspJounce: int = 0x20
SuspSpringForce: int = 0x24
TireContactNormal: int = 0x10
TireFriction: int = 0x1C
TireLateralDir: int = 0x34
TireLongitudinalDir: int = 0x28
WheelRotationAngle: int = 0x70
WheelSpeed: int = 0x6C

def GetVehicleWheelDynamicState(): ...
