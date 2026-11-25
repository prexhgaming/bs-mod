# module: Lua

import sys

LuaChef.LuaCreateType: int = 0x6D46072000
LuaDispatcher.LuaCreateType: int = 0x6D46072000
LuaMType.LuaCreateType: int = 0x6D46072000
LuaNative.LuaCreateType: int = 0x6D46072000
LuaVM.Destroy: int = 0x6D46072000
LuaVM.EnableProfiler: int = 0x6D46072000
LuaVM.Run: int = 0x6D46072000

def GetLua(): ...
