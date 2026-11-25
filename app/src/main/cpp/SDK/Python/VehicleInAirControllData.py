# module: VehicleInAirControllData

import sys

EnableAccelControllPitch: int = 0x19
EnablePitchControll: int = 0x18
EnablePitchTurn: int = 0x1A
PitchControllDelayTime: int = 0x14
PitchTurnAngleThreshold: int = 0xC
PitchTurnSpeed: int = 0x10

def GetVehicleInAirControllData(): ...
