# module: Sound

import sys

SoundComponentExt.AddOutputDevice: int = 0x6D46072000
SoundComponentExt.DelPortal: int = 0x6D46072000
SoundComponentExt.DelRoom: int = 0x6D46072000
SoundComponentExt.DelSpatialObstruction: int = 0x6D46072000
SoundComponentExt.GetActiveGameObjects: int = 0x6D46072000
SoundComponentExt.GetAllGameObjects: int = 0x6D46072000
SoundComponentExt.GetEventIDFromPlayingID: int = 0x6D46072000
SoundComponentExt.GetFNVHashFromString: int = 0x6D46072000
SoundComponentExt.GetGameObjectFromPlayingID: int = 0x6D46072000
SoundComponentExt.GetIsGameObjectActive: int = 0x6D46072000
SoundComponentExt.GetPlayingIDsFromGameObject: int = 0x6D46072000
SoundComponentExt.GetSourcePlayPosition: int = 0x6D46072000
SoundComponentExt.Play3DEventWithSwitch: int = 0x6D46072000
SoundComponentExt.Play3DEventWithSwitches: int = 0x6D46072000
SoundComponentExt.PostTrigger: int = 0x6D46072000
SoundComponentExt.RemoveGameObjectParameter: int = 0x6D46072000
SoundComponentExt.RemoveOutputDevice: int = 0x6D46072000
SoundComponentExt.SetBusConfig: int = 0x6D46072000
SoundComponentExt.SetBusEffect: int = 0x6D46072000
SoundComponentExt.SetEncryptionKey: int = 0x6D46072000
SoundComponentExt.SetGameObjectInRoom: int = 0x6D46072000
SoundComponentExt.SetGameObjectObstructionAndOcclusion: int = 0x6D46072000
SoundComponentExt.SetGameObjectPosition: int = 0x6D46072000
SoundComponentExt.SetGameObjectPositionAndOrientation: int = 0x6D46072000
SoundComponentExt.SetGameObjectPositionAndOrientationBias: int = 0x6D46072000
SoundComponentExt.SetGameObjectVolume: int = 0x6D46072000
SoundComponentExt.SetGameObjectVolumeRamp: int = 0x6D46072000
SoundComponentExt.SetGameObjectWhiteList: int = 0x6D46072000
SoundComponentExt.SetMediaAsyncCallback: int = 0x6D46072000
SoundComponentExt.SetMediaAsyncList: int = 0x6D46072000
SoundComponentExt.SetMediaLookUpTable: int = 0x6D46072000
SoundComponentExt.SetMuteForCategory: int = 0x6D46072000
SoundComponentExt.SetPortal: int = 0x6D46072000
SoundComponentExt.SetPortalObstructionAndOcclusion: int = 0x6D46072000
SoundComponentExt.SetPriorityForCategory: int = 0x6D46072000
SoundComponentExt.SetReflectionsOrder: int = 0x6D46072000
SoundComponentExt.SetRoom: int = 0x6D46072000
SoundComponentExt.SetScalingFactor: int = 0x6D46072000
SoundComponentExt.SetSpatial: int = 0x6D46072000
SoundComponentExt.SetSpatialObstructionBox: int = 0x6D46072000
SoundComponentExt.SetSpatialObstructionPolygon: int = 0x6D46072000
SoundComponentExt.SetState: int = 0x6D46072000
SoundComponentExt.SetSwitch: int = 0x6D46072000
SoundComponentExt.SetVolumeForCategory: int = 0x6D46072000
SoundComponentExt.StartOutputCapture: int = 0x6D46072000
SoundComponentExt.StopOutputCapture: int = 0x6D46072000
SoundComponentExt.VisualizeSpatialHelpers: int = 0x6D46072000

def GetSound(): ...
