# module: GarmentSimulationProperty

import sys

BindingDistance: int = 0x30
BindingStiffness: int = 0x34
BindingTolerance: int = 0x2C
BodyCollisionDetectionScale: int = 0x18
BodyCollisionThickness: int = 0x14
DiheralBendStiffness: int = 0xC
EnableBinding: int = 0x28
EnableSelfCollision: int = 0x1C
IsometricBendStiffness: int = 0x10
SelfCollisionDetectionScale: int = 0x24
SelfCollisionThickness: int = 0x20

def GetGarmentSimulationProperty(): ...
