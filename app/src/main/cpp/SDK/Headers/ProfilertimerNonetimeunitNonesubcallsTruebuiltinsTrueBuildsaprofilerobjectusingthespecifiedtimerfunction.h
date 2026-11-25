class Profiler(timer=None, timeunit=None, subcalls=True, builtins=True)

    Builds a profiler object using the specified timer function {
  uintptr_t The default timer is a fast built-in one based on real time.
    For custom timer functions returning integers, timeunit can
    be a float specifying a scale (i.e. how long each integer unit
    is, in seconds).; // Offset: 0x10 // Global: 0x7026088
};
