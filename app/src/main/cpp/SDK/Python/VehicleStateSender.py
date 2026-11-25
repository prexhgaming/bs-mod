# module: VehicleStateSender

import sys

DebugOutput: int = 0x8D
EnableSend: int = 0x8C
SendInterval: int = 0x88

def GetVehicleStateSender(): ...
