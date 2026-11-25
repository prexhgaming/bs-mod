class VehicleStateFilter {
  uintptr_t ApplyMotion; // Offset: 0x80
  uintptr_t DTimeFactorDownT; // Offset: 0x88
  uintptr_t DTimeFactorKeepT; // Offset: 0x8C
  uintptr_t DTimeFactorStep; // Offset: 0x90
  uintptr_t DTimeFactorUpT; // Offset: 0x84
  uintptr_t DebugOutput; // Offset: 0x81
  uintptr_t DriveRB; // Offset: 0x130
  uintptr_t TimeErrorTH; // Offset: 0x94
  uintptr_t UseTaskGraph; // Offset: 0xF8
  uintptr_t World; // Offset: 0x100
};
