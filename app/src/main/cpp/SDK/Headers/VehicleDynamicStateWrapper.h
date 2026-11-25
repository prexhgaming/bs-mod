class VehicleDynamicStateWrapper {
  uintptr_t CurrentGear; // Offset: 0x44
  uintptr_t DestVehicleForwardSpeed; // Offset: 0x50
  uintptr_t EngineSpeed; // Offset: 0x58
  uintptr_t EngineSpeedDiff; // Offset: 0x5C
  uintptr_t ForwardSpeedSmoothSpeed; // Offset: 0x54
  uintptr_t GarDownPressed; // Offset: 0x42
  uintptr_t GearUpPressed; // Offset: 0x41
  uintptr_t InputAccel; // Offset: 0x60
  uintptr_t InputBrake; // Offset: 0x64
  uintptr_t InputHandBrake; // Offset: 0x68
  uintptr_t InputSteerLeft; // Offset: 0x6C
  uintptr_t InputSteerRight; // Offset: 0x70
  uintptr_t TargetGear; // Offset: 0x48
  uintptr_t UseAutoGears; // Offset: 0x40
  uintptr_t VehicleForwardSpeed; // Offset: 0x4C
  uintptr_t Wheels; // Offset: 0x78
};
