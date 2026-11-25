# module: CharCtrlComponent

import sys

AngularSpeed: int = 0x2E4
AutoUpdate: int = 0x330
CanRideOnCharCtrl: int = 0x2B4
CharOverlapBufferInnerOffset: int = 0x2EC
CharOverlapBufferOffset: int = 0x2F0
ContactOffset: int = 0x2B0
CtrlEntY: int = 0x2FF
DisableSlide: int = 0x33C
DynamicCDRadius: int = 0x30C
Enable: int = 0x2FC
EnableDynamicCollision: int = 0x308
EnableFallDetecting: int = 0x2FE
EnableRideVelocity: int = 0x6D46046000
EnableShapeUpFitting: int = 0x301
EnableStepUpSearch: int = 0x33D
EnableStickToGround: int = 0x300
ForcePassiveMode: int = 0x328
Gravity: int = 0x278
GravityScale: int = 0x3C8
InterpolatePose: int = 0x332
IsInTeleporting: int = 0x3A5
IsSupported: int = 0x3DC
LastRealVel: int = 0x2D4
LinearVel: int = 0x2B8
MaxAllowedDiff: int = 0x32C
MaxAngularSpeed: int = 0x2E8
MaxDroppingVel: int = 0x304
MaxMotionAdvance: int = 0x3D8
MaxPitch: int = 0x314
MaxRoll: int = 0x318
MaxShapeUpAng: int = 0x2A8
MaxSlope: int = 0x2A4
MotionScale: int = 0x2F4
Navigator: int = 0x3E0
NeedPosReady: int = 0x331
NewCollisionResponse: int = 0x333
NewCollisionResponseFactor: int = 0x334
Pos: int = 0x284
PushStrength: int = 0x290
PushUpRatio: int = 0x294
RealVel: int = 0x2C4
RideVelocity: int = 0x6D46046000
ShapeUpFittingByGroundNormalApproximate: int = 0x310
SimLevel: int = 0x2FD
SlideCDTime: int = 0x338
StepOffset: int = 0x2AC
SupportedDelta: int = 0x2F8
Up: int = 0x298
Yaw: int = 0x2E0

def GetCharCtrlComponent(): ...
