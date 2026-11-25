class sys {
  uintptr_t flags

Flags provided through command line arguments or environment vars.; // Offset: 0x12
  uintptr_t float_info

A named tuple holding information about the float type. It contains low level
information about the precision and internal representation. Please study
your system's :file:`float.h` for more information.; // Offset: 0xB
  uintptr_t int_info

A named tuple that holds information about Python's
internal representation of integers.  The attributes are read only.; // Offset: 0x4
  uintptr_t thread_info

A named tuple holding information about the thread implementation.; // Offset: 0x3
  uintptr_t version_info

Version information as a named tuple.; // Offset: 0x5
};
