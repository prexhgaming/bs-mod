class CameraMover {
  uintptr_t BeginDirection; // Offset: 0xC4
  uintptr_t BeginFov; // Offset: 0x100
  uintptr_t BeginTargetPosOffset; // Offset: 0xB8
  uintptr_t BoneName; // Offset: 0x90
  uintptr_t CurrDirection; // Offset: 0xF4
  uintptr_t CurrFov; // Offset: 0x108
  uintptr_t CurrTargetPosOffset; // Offset: 0xE8
  uintptr_t EndDirection; // Offset: 0xDC
  uintptr_t EndFov; // Offset: 0x104
  uintptr_t EndTargetPosOffset; // Offset: 0xD0
  uintptr_t Finished; // Offset: 0x10C
  uintptr_t MoveType; // Offset: 0xA8
  uintptr_t Target; // Offset: 0x120
  uintptr_t TargetPos; // Offset: 0x110
  uintptr_t TargetTransform; // Offset: 0x128
  uintptr_t UseBonePosition; // Offset: 0x88
};
