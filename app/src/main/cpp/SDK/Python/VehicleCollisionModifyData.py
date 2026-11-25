# module: VehicleCollisionModifyData

import sys

ControllFriction: int = 0x23
ControllInvInertiaScale: int = 0x21
ControllInvMassScale: int = 0x22
ControllMaxImpulse: int = 0x24
ControllNormal: int = 0x25
ControllRestitution: int = 0x20
DynamicFriction: int = 0x10
EnableFilter: int = 0x26
Enabled: int = 0x27
FilterData: int = 0x1C
InvMassScale: int = 0xC
MaxImpulse: int = 0x18
StaticFriction: int = 0x14

def GetVehicleCollisionModifyData(): ...
