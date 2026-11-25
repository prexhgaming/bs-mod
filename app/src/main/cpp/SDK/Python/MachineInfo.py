# module: MachineInfo

import sys

Addresses: int = 0x48
Applications: int = 0x60
MemoryPageSize: int = 0x3C
MemoryTotal: int = 0x40
Online: int = 0x31
ProcessId: int = 0x38
ProcessorCore: int = 0x34
ProcessorGroup: int = 0x32
Remote: int = 0x30
User: int = 0x18

def GetMachineInfo(): ...
