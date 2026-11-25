class VehicleEngine {
  uintptr_t DampingRateFullThrottle; // Offset: 0x4C
  uintptr_t DampingRateZeroThrottleClutchDisengaged; // Offset: 0x54
  uintptr_t DampingRateZeroThrottleClutchEngaged; // Offset: 0x50
  uintptr_t MOI; // Offset: 0x40
  uintptr_t MaxOmega; // Offset: 0x48
  uintptr_t PeakTorque; // Offset: 0x44
  uintptr_t TorqueCurveEngineSpeed; // Offset: 0x58
  uintptr_t TorqueCurveEngineTorque; // Offset: 0x70
};
