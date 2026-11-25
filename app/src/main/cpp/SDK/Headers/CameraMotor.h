class CameraMotor {
  uintptr_t BackToStart; // Offset: 0xD4
  uintptr_t CurrMoveVector; // Offset: 0xA0
  uintptr_t CurrRotateAngle; // Offset: 0xC8
  uintptr_t EndFrame; // Offset: 0xB4
  uintptr_t EscapedTime; // Offset: 0xD8
  uintptr_t Finished; // Offset: 0x90
  uintptr_t FrameCount; // Offset: 0xB8
  uintptr_t InterpolateMode; // Offset: 0x8C
  uintptr_t Invalid; // Offset: 0x88
  uintptr_t MoveVector; // Offset: 0x94
  uintptr_t MoveVectorLen; // Offset: 0xB0
  uintptr_t MoveVectorType; // Offset: 0xAC
  uintptr_t RotateAngle; // Offset: 0xBC
  uintptr_t TotalTime; // Offset: 0xDC
};
