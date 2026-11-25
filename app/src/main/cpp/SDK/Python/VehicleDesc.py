# module: VehicleDesc

import sys

Chassis: int = 0x120
ChassisMaterials: int = 0x2B0
ChassisShapes: int = 0x2E0
CollisionInfo: int = 0x2F8
InAirControllParams: int = 0x320
KeySmooth: int = 0x230
MotorBikeBalanceParams: int = 0x2FC
PadSmooth: int = 0x258
SteerRatio: int = 0x280
SupsStickGroundParams: int = 0x318
WheelAndSuspension: int = 0x148
WheelMaterials: int = 0x298
WheelShapes: int = 0x2C8

def GetVehicleDesc(): ...
