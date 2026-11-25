# module: VehicleSurfaceToTireFrictionPairsConfig

import sys

DefaultFriction: int = 0x48
SurfaceTypeCnt: int = 0x44
TireTypeCnt: int = 0x40

def GetVehicleSurfaceToTireFrictionPairsConfig(): ...
