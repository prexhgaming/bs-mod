#!/usr/bin/env python3
"""
find_pyrun.py -- locate PyRun_StringFlags inside a stripped ARM libGame.so.

Why this works
--------------
The game statically links CPython and strips its symbols, so there is no name to
look up. But the *call signature* is fixed and survives stripping:

    PyObject *PyRun_StringFlags(const char *str, int start,
                                PyObject *globals, PyObject *locals,
                                PyObject *flags)

and PyRun_SimpleStringFlags -- which every embedder (this game included) calls
for its own scripts -- hardcodes:

    PyRun_StringFlags(cmd, Py_file_input, main_dict, main_dict, flags)

Two invariants follow, and together they are specific enough to single out one
function in 50+ MB of code:

  1. start == Py_file_input == 257, so the second argument is always loaded as
     `movw r1, #0x101` (Thumb: bytes 40 F2 01 11).
  2. globals == locals, so r2 and r3 hold the SAME value, which a compiler
     almost always expresses as "both come from the same register".

So: find every `movw r1,#0x101`, follow it to the next `bl`, resolve what r2 and
r3 were last written from, and keep the BL whose r2/r3 source registers are
identical. Group by target address; the function the most qualifying call sites
agree on is PyRun_StringFlags.

Do NOT use "string xref" instead. `"<string>"` is not referenced directly by
PyRun_StringFlags: CPython 3.12+ reaches that literal through a GOT slot for
_PyRuntime (LDR r0,[pc,#imm] / ADD r0,pc / LDR r0,[r0] / ADD.W r5,r0,#imm), so
address-of-literal and relocation sweeps both come up empty. The call signature
above works and was validated against a real binary.

Usage
-----
    python3 find_pyrun.py libGame.so
    python3 find_pyrun.py libGame.so --top 5
    python3 find_pyrun.py libGame.so --explain 0x24dbcf0     # dump one candidate

The reported offset is what goes into kNexoraPyRunOffset in main.cpp: it is the
function's address relative to the load base, i.e. its ELF vaddr. On a 32-bit
Thumb build the low bit is NOT set (the hook layer adds it).
"""

import argparse
import struct
import sys
from bisect import bisect_right

EM_ARM = 40
PT_LOAD = 1
SHT_SYMTAB = 2
SHT_REL = 9
SHT_RELA = 4
SHT_ARM_EXIDX = 0x70000001


# --------------------------------------------------------------------------- ELF

def load_elf(path):
    """Minimal ELF32 little-endian parser.

    Returns dict with the raw bytes plus the section/program-header tables.
    Only what this tool needs: sections (for .text/.rel.dyn bounds) and PT_LOAD
    segments (for the vaddr<->file-offset mapping).
    """
    with open(path, "rb") as f:
        data = f.read()

    if data[:4] != b"\x7fELF":
        sys.exit("not an ELF file: %s" % path)
    if data[4] != 1:
        sys.exit("ELF is not 32-bit -- this tool targets armeabi-v7a builds")
    if data[5] != 1:
        sys.exit("ELF is big-endian -- only little-endian ARM is supported")
    machine = struct.unpack_from("<H", data, 18)[0]
    if machine != EM_ARM:
        sys.exit("e_machine=%d is not EM_ARM; nothing to scan" % machine)

    e_shoff = struct.unpack_from("<I", data, 0x20)[0]
    e_shentsize = struct.unpack_from("<H", data, 0x2E)[0]
    e_shnum = struct.unpack_from("<H", data, 0x30)[0]
    e_shstrndx = struct.unpack_from("<H", data, 0x32)[0]
    e_phoff = struct.unpack_from("<I", data, 0x1C)[0]
    e_phentsize = struct.unpack_from("<H", data, 0x2A)[0]
    e_phnum = struct.unpack_from("<H", data, 0x2C)[0]

    sections = []
    for i in range(e_shnum):
        base = e_shoff + i * e_shentsize
        if base + 0x28 > len(data):
            break
        (name, stype, flags, addr, off, size, link, info, align,
         entsize) = struct.unpack_from("<IIIIIIIIII", data, base)
        sections.append({
            "name_off": name, "type": stype, "flags": flags, "addr": addr,
            "off": off, "size": size, "link": link, "info": info,
            "entsize": entsize,
        })

    if sections and e_shstrndx < len(sections):
        stroff = sections[e_shstrndx]["off"]
        for s in sections:
            end = data.find(b"\x00", stroff + s["name_off"])
            s["name"] = data[stroff + s["name_off"]:end].decode("latin-1")
    else:
        for s in sections:
            s["name"] = ""

    segments = []
    for i in range(e_phnum):
        base = e_phoff + i * e_phentsize
        if base + 0x20 > len(data):
            break
        p_type, p_offset, p_vaddr, p_paddr, p_filesz, p_memsz, p_flags, \
            p_align = struct.unpack_from("<IIIIIIII", data, base)
        segments.append({
            "type": p_type, "off": p_offset, "vaddr": p_vaddr,
            "filesz": p_filesz, "memsz": p_memsz,
        })

    return {"data": data, "sections": sections, "segments": segments}


def exec_section(elf):
    """The section to scan: .text if named, else the biggest executable one."""
    for s in elf["sections"]:
        if s["name"] == ".text":
            return s
    cands = [s for s in elf["sections"] if s["flags"] & 0x4 and s["type"] == 1]
    if not cands:
        sys.exit("no executable section found")
    return max(cands, key=lambda s: s["size"])


def va_to_off(elf, va):
    """Map a virtual address to a file offset via PT_LOAD segments."""
    for seg in elf["segments"]:
        if seg["type"] != PT_LOAD:
            continue
        start = seg["vaddr"]
        if start <= va < start + seg["filesz"]:
            return seg["off"] + (va - start)
    return None


# ---------------------------------------------------------------- instruction access

class Code:
    """Read-only view of the executable section with VA<->offset helpers."""

    def __init__(self, elf, text):
        self.elf = elf
        self.data = elf["data"]
        self.va = text["addr"]
        self.size = text["size"]
        self.off = text["off"]
        self.end = self.va + self.size

    def has(self, va, n=4):
        return self.va <= va < self.end - n

    def u16(self, va):
        return struct.unpack_from("<H", self.data, self.off + (va - self.va))[0]

    def u32(self, va):
        return struct.unpack_from("<I", self.data, self.off + (va - self.va))[0]

    def in_text(self, va):
        return self.va <= va < self.end


# ------------------------------------------------------------------- instruction set
#
# Only the handful of encodings the scan needs. Full ARM/Thumb decoding is not
# the goal -- if you extend this, run tmp/thumb_check.py-style round-trip tests,
# because an off-by-one in a BL immediate silently points at mid-function code.

def thumb_movw(hw1, hw2):
    """Thumb-2 MOVW: 11110 i 100100 imm4 | 0 imm3 Rd imm8 -> (rd, imm16) or None."""
    if (hw1 & 0xFB50) != 0xF240 or (hw2 & 0x8000) != 0:
        return None
    imm4 = hw1 & 0xF
    imm3 = (hw2 >> 12) & 0x7
    rd = (hw2 >> 8) & 0xF
    imm8 = hw2 & 0xFF
    return rd, (imm4 << 12) | (imm3 << 8) | imm8


def arm_movw(w):
    """A32 MOVW (cond=AL): 1110 0011 0000 imm4 | Rd imm12."""
    if (w & 0xFFF00000) != 0xE30000000:
        return None
    rd = (w >> 12) & 0xF
    imm16 = ((w >> 16) & 0xF) << 12 | (w & 0xFFF)
    return rd, imm16


def thumb_bl(c, va):
    """Thumb-2 BL -> absolute target, or None. imm32 sign-extends."""
    hw1 = c.u16(va)
    if (hw1 & 0xF800) != 0xF000:
        return None
    hw2 = c.u16(va + 2)
    if (hw2 & 0xD000) != 0xD000:  # BL requires bits 15:14=11, bit 12=1
        return None
    s = (hw1 >> 10) & 1
    j1 = (hw2 >> 13) & 1
    j2 = (hw2 >> 11) & 1
    i1 = 1 ^ (j1 ^ s)
    i2 = 1 ^ (j2 ^ s)
    imm10 = hw1 & 0x3FF
    imm11 = hw2 & 0x7FF
    imm32 = (s << 24) | (i1 << 23) | (i2 << 22) | (imm10 << 12) | (imm11 << 1)
    if s:
        imm32 -= 1 << 25
    return (va + 4 + imm32) & 0xFFFFFFFF


def arm_bl(c, va):
    """A32 BL: cond 1011 imm24 -> absolute target."""
    w = c.u32(va)
    if (w & 0x0F000000) != 0x0B000000:
        return None
    imm = w & 0xFFFFFF
    if imm & 0x800000:
        imm -= 1 << 24
    return (va + 8 + (imm << 2)) & 0xFFFFFFFF


def is_thumb_func(c, va):
    """Heuristic: Thumb code usually starts with PUSH {..., lr} (B5xx)."""
    return (c.u16(va) & 0xFF00) == 0xB500


def is_arm_func(c, va):
    """Heuristic: A32 prologue pushes lr, e.g. E92D4xxx."""
    return (c.u32(va) & 0xFFFF0000) == 0xE92D0000


# ------------------------------------------------------------- register backtracking

# A deliberately small opcode table: "which register does this instruction write,
# and from which register does the value come". Enough to trace `mov`/`add`/`ldr`
# chains a compiler emits while setting up call arguments; not a disassembler.

def thumb_writes(c, va):
    """-> (dst_reg, src_reg_or_None) if the Thumb insn at va writes a low reg."""
    hw1 = c.u16(va)
    hw2 = c.u16(va + 2)

    if (hw1 & 0xFFC0) == 0x4600:                       # MOV (register)
        rm = (hw2 >> 3) & 0xF
        rd = (hw2 & 0x7) | ((hw1 & 0x80) >> 4)
        return rd, rm
    if (hw1 & 0xF800) == 0x0800:                       # LDR (literal)
        return (hw1 >> 8) & 0x7, None
    if (hw1 & 0xF8C0) == 0xF8C0 and (hw2 & 0x8000) == 0:  # LDR (immediate)
        return (hw2 >> 12) & 0xF, None
    if (hw1 & 0xFC00) == 0x5800:                       # LDR (register offset)
        return (hw1 >> 8) & 0x7, None
    m = thumb_movw(hw1, hw2)
    if m:
        return m[0], None
    if (hw1 & 0xFF80) == 0x4480:                       # ADD (register)
        d = (hw1 & 0x7) | ((hw2 >> 4) & 0x8)
        return d, None
    if (hw1 & 0xF800) == 0x2000:                       # MOVS (immediate)
        return (hw1 >> 8) & 0x7, None
    return None


def arm_writes(c, va):
    w = c.u32(va)
    if (w & 0x0FE00000) == 0x01A00000:                 # MOV (register)
        return (w & 0xF), ((w >> 16) & 0xF)
    if (w & 0x0C100000) == 0x04100000:                 # LDR
        return ((w >> 12) & 0xF), None
    if (w & 0x0FEF0000) == 0x028D0000:                 # ADD rd, sp, #imm
        return ((w >> 12) & 0xF), None
    m = arm_movw(w)
    if m:
        return m[0], None
    return None


def sources_before(c, bl_va, mode, back=0x60):
    """Which register fed r2 and r3 at this BL.

    Walks backwards from the call and records the LAST write to each register,
    which is the value that reaches the call. Returns {2: src, 3: src}.
    """
    found = {}
    va = bl_va - _insn_back(mode)
    while va >= bl_va - back and c.in_text(va):
        info = thumb_writes(c, va) if mode == "thumb" else arm_writes(c, va)
        if info:
            dst, src = info
            if dst in (2, 3) and dst not in found:
                found[dst] = src
            if len(found) == 2:
                break
        va -= _insn_back(mode)
    return found


def _insn_back(mode):
    return 2 if mode == "thumb" else 4


# --------------------------------------------------------------------------- scan

def scan(elf, verbose=False):
    text = exec_section(elf)
    c = Code(elf, text)
    print("scanning %s: VA 0x%08x .. 0x%08x (%.1f MB)"
          % (text["name"], c.va, c.end, c.size / 1048576.0))

    # Pass 1: every place that materialises 257 into r1.
    #
    # MOVW with a fully known immediate and register is a single fixed encoding,
    # so search the raw bytes instead of decoding 42M instructions in Python.
    #   Thumb: movw r1,#0x101 -> halfwords F240 1101 -> 40 F2 01 11
    #   A32:   movw r1,#0x101 -> word     E301 1101 -> 01 11 01 E3
    seeds = []
    blob = c.data
    for pattern, mode in ((b"\x40\xF2\x01\x11", "thumb"),
                          (b"\x01\x11\x01\xE3", "arm")):
        i = blob.find(pattern, c.off, c.off + c.size)
        while i != -1:
            va = c.va + (i - c.off)
            # Cross-check with the decoder so a pattern hit that is really a
            # different instruction (e.g. data inside .text) is dropped.
            if mode == "thumb":
                m = thumb_movw(c.u16(va), c.u16(va + 2))
            else:
                m = arm_movw(c.u32(va))
            if m and m[0] == 1 and m[1] == 0x101:
                seeds.append((va, mode))
            i = blob.find(pattern, i + 1, c.off + c.size)
    print("seed sites (movw r1,#0x101): %d" % len(seeds))

    # Pass 2: follow each seed to the next call, see where r2/r3 came from.
    votes = {}
    detail = {}
    for seed_va, mode in seeds:
        bl_va, target = _next_call(c, seed_va, mode)
        if not target or not c.in_text(target):
            continue
        srcs = sources_before(c, bl_va, mode)
        r2, r3 = srcs.get(2), srcs.get(3)
        same = r2 is not None and r2 == r3 and r2 != 0
        detail.setdefault(target, []).append((bl_va, mode, r2, r3, same))
        # `globals == locals` is the discriminator; a call without it is noise.
        votes[target] = votes.get(target, 0) + (5 if same else 1)

    ranked = sorted(votes.items(), key=lambda kv: (-kv[1], kv[0]))
    strict = [(t, s) for t, s in ranked
              if any(d[4] for d in detail.get(t, []))]
    return c, ranked, strict, detail


def _next_call(c, start_va, mode):
    """First BL/BLX at or after start_va, within a plausible window."""
    va = start_va
    for _ in range(0x60):
        if mode == "thumb":
            t = thumb_bl(c, va)
            if t:
                return va, t
        else:
            t = arm_bl(c, va)
            if t:
                return va, t
        va += _insn_back(mode)
        if va >= c.end - 8:
            break
    return None, None


def explain(c, detail, target):
    print("\n=== candidate 0x%08x ===" % target)
    if not (c.in_text(target)):
        print("  outside .text")
        return
    print("  prologue: %s" % ("thumb (push {..,lr})" if is_thumb_func(c, target)
                              else "arm (push lr)" if is_arm_func(c, target)
                              else "unrecognised"))
    for bl_va, mode, r2, r3, same in detail.get(target, []):
        print("  call @0x%08x  %-5s  r2<-%s r3<-%s  %s"
              % (bl_va, mode,
                 "r%d" % r2 if r2 is not None else "?",
                 "r%d" % r3 if r3 is not None else "?",
                 "MATCH" if same else ""))
    # Callee count is a cheap sanity check: PyRun_StringFlags has a handful of
    # calls (arena, parser, run_mod, arena free).
    calls = []
    va = target
    for _ in range(0xC0):
        t = thumb_bl(c, va)
        if t:
            calls.append(t)
        va += 2
        if va >= c.end - 8:
            break
    print("  callees in first 0xc0 bytes: %d" % len(calls))
    for t in calls[:6]:
        print("    bl 0x%08x" % t)


def main():
    ap = argparse.ArgumentParser(
        description="Locate PyRun_StringFlags in a stripped ARM libGame.so")
    ap.add_argument("so", help="path to libGame.so")
    ap.add_argument("--top", type=int, default=5,
                    help="how many candidates to print (default 5)")
    ap.add_argument("--explain", default=None,
                    help="dump call sites for one candidate offset, e.g. 0x24dbcf0")
    args = ap.parse_args()

    elf = load_elf(args.so)
    c, ranked, strict, detail = scan(elf)

    if args.explain:
        explain(c, detail, int(args.explain, 0) & ~1)
        return

    print("\n=== candidates where globals == locals (r2 == r3) ===")
    if not strict:
        print("  none -- this build may not call PyRun_SimpleStringFlags, or")
        print("  globals/locals are assembled differently. See the ranked list")
        print("  below and verify a candidate by hand (--explain).")
    for target, score in strict[:args.top]:
        n = sum(1 for d in detail[target] if d[4])
        print("  +0x%08x  %d qualifying call(s)   -> kNexoraPyRunOffset = 0x%x"
              % (target, n, target))

    print("\n=== ranked by vote weight (top %d) ===" % args.top)
    for target, score in ranked[:args.top]:
        mark = "  <-- strict match" if any(
            d[4] for d in detail.get(target, [])) else ""
        print("  +0x%08x  weight %-3d%s" % (target, score, mark))

    if strict:
        best = strict[0][0]
        print("\nBest candidate: kNexoraPyRunOffset = 0x%x" % best)
        print("Verify before trusting it:")
        print("  python3 %s %s --explain 0x%x" % (sys.argv[0], args.so, best))
        print("Confirm in logcat: 'hook confirmed: start=257 src=...'")
    else:
        print("\nNo strict match. Widen with --explain on the ranked list, or")
        print("look for a movw r1,#0x100 (Py_single_input) / #0x102 (Py_eval_input)")
        print("call instead -- see the header comment.")


if __name__ == "__main__":
    main()
