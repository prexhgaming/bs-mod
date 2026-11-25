class WindFieldData {
  uintptr_t AdvectionBlendParam; // Offset: 0x80
  uintptr_t AmbientNoiseTexId; // Offset: 0x40
  uintptr_t AmbientTexToWorldMap; // Offset: 0x24
  uintptr_t AmbientUVSize; // Offset: 0x30
  uintptr_t AmbientWind; // Offset: 0x18
  uintptr_t AmbientWindDisturbance; // Offset: 0x50
  uintptr_t DeltaTime; // Offset: 0x78
  uintptr_t DiffusionRate; // Offset: 0x7C
  uintptr_t EnableAdvection; // Offset: 0x8C
  uintptr_t EnableDiffusion; // Offset: 0x8D
  uintptr_t EnableNoiseAmbient; // Offset: 0x8F
  uintptr_t EnableProjection; // Offset: 0x8E
  uintptr_t FieldSize; // Offset: 0x68
  uintptr_t GridLength; // Offset: 0x74
  uintptr_t MinPos; // Offset: 0x5C
  uintptr_t NBDiffusionIter; // Offset: 0x84
  uintptr_t NBProjectIter; // Offset: 0x88
  uintptr_t NoiseSampleLevel; // Offset: 0x54
  uintptr_t NoiseUVResetTime; // Offset: 0x58
  uintptr_t constantWind; // Offset: 0xC
};
