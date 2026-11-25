class NoiseController {
  uintptr_t CurrentPosition; // Offset: 0xE4
  uintptr_t FinalPitch; // Offset: 0x120
  uintptr_t FinalPosition; // Offset: 0x114
  uintptr_t FinalRoll; // Offset: 0x128
  uintptr_t FinalYaw; // Offset: 0x124
  uintptr_t PositionNoise; // Offset: 0x90
  uintptr_t PositionNoiseFPS; // Offset: 0xA8
  uintptr_t PositionNoiseWeight; // Offset: 0xAC
  uintptr_t RotationNoise; // Offset: 0xB8
  uintptr_t RotationNoiseFPS; // Offset: 0xD0
  uintptr_t RotationNoiseWeight; // Offset: 0xD4
};
