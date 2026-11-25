class itemgetter(item, {
  uintptr_t ..) --> itemgetter object

Return a callable object that fetches the given item(s) from its operand.
After f = itemgetter(2), the call f(r) returns r[2].
After g = itemgetter(2, 5, 3), the call g(r) returns (r[2], r[5], r[3]); // Offset: 0x10 // Global: 0x1666C7B
};
