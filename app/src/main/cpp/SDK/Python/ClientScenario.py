# module: ClientScenario

import sys

ActiveWorld: int = 0x200
AsyncLoadWorld: int = 0x241
DelayDeserialization: int = 0x240
DeserializationTimeSlice: int = 0x238
LinkedWorlds: int = 0x210
Sound: int = 0x208

def GetClientScenario(): ...
