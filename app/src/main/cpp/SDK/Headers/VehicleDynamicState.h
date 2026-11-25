class VehicleDynamicState {
  uintptr_t DestVehicleForwardSpeed; // Offset: 0x10
  uintptr_t EngineSpeed; // Offset: 0x18
  uintptr_t EngineSpeedDiff; // Offset: 0x1C
  uintptr_t ForwardSpeedSmoothSpeed; // Offset: 0x14
  uintptr_t InputAccel; // Offset: 0x20
  uintptr_t InputBrake; // Offset: 0x24
  uintptr_t InputHandBrake; // Offset: 0x28
  uintptr_t InputSteerLeft; // Offset: 0x2C
  uintptr_t InputSteerRight; // Offset: 0x30
  uintptr_t VehicleForwardSpeed; // Offset: 0xC
  uintptr_t Wheels; // Offset: 0x38
};
