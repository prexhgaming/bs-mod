class statvfs_result: Result from statvfs or fstatvfs {
  uintptr_t This object may be accessed either as a tuple of
  (bsize, frsize, blocks, bfree, bavail, files, ffree, favail, flag, namemax),
or via the attributes f_bsize, f_frsize, f_blocks, f_bfree, and so on.

See os.statvfs for more information.; // Offset: 0xA
};
