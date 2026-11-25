# module: VehicleInAirControllState

import sys

IsActivated: int = 0x10
IsPitchTurnActivated: int = 0x11
PitchTurnStartYaw: int = 0xC

def GetVehicleInAirControllState(): ...
