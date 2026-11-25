# module: VehicleInAirControllParamsWrapper

import sys

EnableAccelControllPitch: int = 0x6D46046000
EnablePitchControll: int = 0x6D46046000
EnablePitchTurn: int = 0x6D46046000
PitchControllAccel: int = 0x6D46046000
PitchControllDelayTime: int = 0x6D46046000
PitchControllSpeed: int = 0x6D46046000
PitchTurnAngleThreshold: int = 0x6D46046000
PitchTurnSpeed: int = 0x6D46046000
RollThreshold: int = 0x6D46046000

def GetVehicleInAirControllParamsWrapper(): ...
