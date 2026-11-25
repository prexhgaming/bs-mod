# module: CameraController

import sys

AffiliatedCamera: int = 0x470
ControlledCamera: int = 0x468
Detector: int = 0x420
EarlyTick: int = 0x431
EnableAffiliatedFov: int = 0x4EA
EnableKeyframeEvent: int = 0x4EC
EnableRecord: int = 0x480
LocateBlendTree: int = 0x450
MaxRecordNum: int = 0x484
PlacerCache: int = 0x438
PlatformEntity: int = 0x428
PostProcessorBlendTree: int = 0x460
RecordedFov: int = 0x4D0
RecordedTransform: int = 0x488
SensorBlendTree: int = 0x458
Suspend: int = 0x430
SweepShape: int = 0x400
TickRole: int = 0x418
VirtualCtrlCamera: int = 0x478

def GetCameraController(): ...
