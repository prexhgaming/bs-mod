# module: sqlite3

import sys

Blob: int = 0x4180
Connection: int = 0x4500
Cursor: int = 0x4500
PrepareProtocol: int = 0x4100
Row: int = 0x4500
Statement: int = 0x4180

def Getsqlite3(): ...
