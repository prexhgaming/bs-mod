# module: VehicleConfig

import sys

BalanceParams: int = 0xD4
Chassis: int = 0xC8
ChassisMaterials: int = 0x80
ChassisShapes: int = 0xB0
CollisionInfo: int = 0xD0
Drive: int = 0x58
InAirControllParams: int = 0xF8
KeySmooth: int = 0x40
PadSmooth: int = 0x48
SteerRatio: int = 0x50
SuspStickParams: int = 0xF0
WheelAndSuspension: int = 0x60
WheelMaterials: int = 0x68
WheelShapes: int = 0x98

def GetVehicleConfig(): ...
