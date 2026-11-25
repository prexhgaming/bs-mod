# module: FlareProperty

import sys

Distance: int = 0xC
FlareColor: int = 0x10
FlareIntensity: int = 0x1C
SizeInPixels: int = 0x20
SizeInPixelsH: int = 0x24
flareIndex: int = 0x28

def GetFlareProperty(): ...
