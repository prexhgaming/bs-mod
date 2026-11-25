# module: SkeletonSystemSetting

import sys

CharacterCoord: int = 0x288
Head: int = 0xC0
LCalf: int = 0x1E0
LClavicle: int = 0xD8
LFinger0: int = 0x138
LFoot: int = 0x1F8
LForearm: int = 0x108
LHand: int = 0x120
LThigh: int = 0x1C8
LToe0: int = 0x210
LUpperArm: int = 0xF0
Neck: int = 0xA8
Pelvis: int = 0x48
RCalf: int = 0x240
RClavicle: int = 0x150
RFinger0: int = 0x1B0
RFoot: int = 0x258
RForearm: int = 0x180
RHand: int = 0x198
RThigh: int = 0x228
RToe0: int = 0x270
RUpperArm: int = 0x168
Root: int = 0x18
RootMode: int = 0x30
Spine: int = 0x60
Spine1: int = 0x78
Spine2: int = 0x90

def GetSkeletonSystemSetting(): ...
