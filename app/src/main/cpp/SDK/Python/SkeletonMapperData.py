# module: SkeletonMapperData

import sys

ChainMappings: int = 0x38
KeepUnmappedLocal: int = 0x68
MappingType: int = 0x69
SimpleMappings: int = 0x20
UnmappedBones: int = 0x50

def GetSkeletonMapperData(): ...
