# module: TwParticleElementTracker

import sys

AccelerationControl: int = 0x94
AutoClear: int = 0x5B
AutoReverse: int = 0x59
AutoStop: int = 0x5A
Delay: int = 0x50
Direction: int = 0x6C
Distance: int = 0x78
EndRadius: int = 0x80
LocalMotion: int = 0x5C
Loop: int = 0x58
Orientation: int = 0x5D
Period: int = 0x54
SpinSpeed: int = 0x90
StartOrbitPhase: int = 0x84
StartPoint: int = 0x60
StartRadius: int = 0x7C
StartSpinPhase: int = 0x8C
Turns: int = 0x88

def GetTwParticleElementTracker(): ...
