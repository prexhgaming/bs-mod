class The object used to calculate HMAC of a message {
  uintptr_t Methods:

update() -- updates the current digest with an additional string
digest() -- return the current digest value
hexdigest() -- return the current digest as a string of hexadecimal digits
copy() -- return a copy of the current hash object

Attributes:

name -- the name, including the hash algorithm used by this object
digest_size -- number of bytes in digest() output; // Offset: 0x10
};
