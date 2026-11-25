# module: VehicleDriveDynamicData

import sys

AutoBoxSwitchTime: int = 0x50
CurrentGear: int = 0x40
EngineSpeed: int = 0x48
GearDownPressed: int = 0x56
GearSwitchTime: int = 0x4C
GearUpPressed: int = 0x55
TargetGear: int = 0x44
UseAutoGears: int = 0x54

def GetVehicleDriveDynamicData(): ...
