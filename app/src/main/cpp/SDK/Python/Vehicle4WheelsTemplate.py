# module: Vehicle4WheelsTemplate

import sys

InAirControll: int = 0x224
KeySmooth: int = 0x1A0
MotorBikeBalance: int = 0x208
OAData: int = 0x240
PadSmooth: int = 0x1C8
RBGuid: int = 0x10
SteadyData: int = 0x274
SteerRatio: int = 0x1F0
VehicleDrive: int = 0x20
WheelsSimData: int = 0x140

def GetVehicle4WheelsTemplate(): ...
