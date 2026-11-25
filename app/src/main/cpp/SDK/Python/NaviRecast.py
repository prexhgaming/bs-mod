# module: NaviRecast

import sys

RecastMapAutoLoader.AddAuxFocusArea: int = 0x6D46072000
RecastMapAutoLoader.ClearAllAuxFocusArea: int = 0x6D46072000
RecastMapAutoLoader.Finalize: int = 0x6D46072000
RecastMapAutoLoader.GetFocusAreaCenters: int = 0x6D46072000
RecastMapAutoLoader.Initial: int = 0x6D46072000
RecastMapAutoLoader.IsInitialized: int = 0x6D46072000
RecastMapDisplayerComponent.EnterSpace: int = 0x6D46072000
RecastMapDisplayerComponent.LeaveSpace: int = 0x6D46072000
RecastMapDisplayerComponent.Refresh: int = 0x6D46072000
RecastMapDisplayerComponent.SetMapEntityTransfrom: int = 0x6D46072000

def GetNaviRecast(): ...
