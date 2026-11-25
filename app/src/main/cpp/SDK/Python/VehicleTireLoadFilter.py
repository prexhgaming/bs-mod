# module: VehicleTireLoadFilter

import sys

MaxFilteredNormalisedLoad: int = 0x4C
MaxNormalisedLoad: int = 0x48
MinFilteredNormalisedLoad: int = 0x44
MinNormalisedLoad: int = 0x40

def GetVehicleTireLoadFilter(): ...
