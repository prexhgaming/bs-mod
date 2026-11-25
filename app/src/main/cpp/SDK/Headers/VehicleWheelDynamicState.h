class VehicleWheelDynamicState {
  uintptr_t CamberAngle; // Offset: 0x4C
  uintptr_t LateralSlip; // Offset: 0x44
  uintptr_t LocalPose; // Offset: 0x50
  uintptr_t LongitudinalSlip; // Offset: 0x40
  uintptr_t SteerAngle; // Offset: 0x48
  uintptr_t SuspJounce; // Offset: 0x20
  uintptr_t SuspSpringForce; // Offset: 0x24
  uintptr_t TireContactNormal; // Offset: 0x10
  uintptr_t TireFriction; // Offset: 0x1C
  uintptr_t TireLateralDir; // Offset: 0x34
  uintptr_t TireLongitudinalDir; // Offset: 0x28
  uintptr_t WheelRotationAngle; // Offset: 0x70
  uintptr_t WheelSpeed; // Offset: 0x6C
};
