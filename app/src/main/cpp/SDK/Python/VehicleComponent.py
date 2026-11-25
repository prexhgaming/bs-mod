# module: VehicleComponent

import sys

Accel: int = 0x510
BalanceState: int = 0x238
Brake: int = 0x511
BrakeSound: int = 0x3A8
BrakeSoundVolume: int = 0x3F8
BrakeThreshold: int = 0x3DC
CollisionFilterInfo: int = 0x6D46046000
EnableInterpolatePose: int = 0x51F
EnableUpdateSkeleton: int = 0x516
EnableVehicleEngine: int = 0x6D46046000
EngineRunSound: int = 0x360
EngineRunSoundVolume: int = 0x3EC
EngineSoundRPMRatio: int = 0x3CC
EngineSpeedMin: int = 0x3C8
EngineSpeedThreshold: int = 0x3C4
EngineStartSound: int = 0x348
EngineStartSoundVolume: int = 0x3E8
ForwardSpeed: int = 0x3E4
ForwardSpeedThreshold: int = 0x3C0
Handbrake: int = 0x512
HoldSoundEvent: int = 0x51E
InAirControllParams: int = 0x248
InAirControllState: int = 0x26C
IsContacted: int = 0x51B
OAParams: int = 0x298
OAState: int = 0x2D4
PadAccel: int = 0x500
PadBrake: int = 0x504
PadHandbrake: int = 0x508
PadSteer: int = 0x50C
RPMMax: int = 0x3E0
RearRightWheelOffset: int = 0x3FC
SteadyParams: int = 0x280
SteerLeft: int = 0x513
SteerRight: int = 0x514
SystemDataContainer: int = 0x4D8
UseKeyboard: int = 0x515
VehicleExhaust: int = 0x330
VehicleExhaustParticle: int = 0x438
VehicleState: int = 0x1E0
WheelDust: int = 0x300
WheelDustParticle: int = 0x428
WheelSlideSound: int = 0x378
WheelSlideSoundVolume: int = 0x3F0
WheelSlipRatioThreshold: int = 0x3D0
WheelSuspensionCompressedThreshold: int = 0x3D8
WheelSuspensionElongatedThreshold: int = 0x3D4
WheelSuspensionSound: int = 0x390
WheelSuspensionSoundVolume: int = 0x3F4
WheelTrail: int = 0x318
WheelTrailParticle: int = 0x430

def GetVehicleComponent(): ...
