# module: FlamencoHairParameter

import sys

AnchorBoneName: int = 0x88
GlobalDamping: int = 0xD4
GlobalShapeMatchingFactor: int = 0xE0
GravityScale: int = 0xD0
GuideHairCount: int = 0x48
HairLengthScale: int = 0x68
IterationCount: int = 0xDC
LocalBound: int = 0x6C
LodDistance: int = 0x4C
LodFator: int = 0x50
NormalHairCount: int = 0x44
RefMatrix: int = 0xA0
Scale: int = 0x40
Spheres: int = 0x100
TranslationScale: int = 0xD8
TwistStiffnessFactor: int = 0xF8
VolumeCollisionForce: int = 0xFC

def GetFlamencoHairParameter(): ...
