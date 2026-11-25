class methodcaller(name, {
  uintptr_t ..) --> methodcaller object

Return a callable object that calls the given method on its operand.
After f = methodcaller('name'), the call f(r) returns r.name().
After g = methodcaller('name', 'date', foo=1), the call g(r) returns
r.name('date', foo=1).; // Offset: 0x10 // Global: 0x165B67E
};
