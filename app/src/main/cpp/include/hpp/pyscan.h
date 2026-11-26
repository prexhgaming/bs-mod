#pragma once

// Runtime discovery of the game's PyRun_StringFlags.
//
// The address is NOT stable across game builds: libGame.so statically links a
// renamed/obfuscated CPython, so a hardcoded `libGameBase + offset` has to be
// re-found by hand after every update. Find it structurally at runtime instead.
//
// Supported: arm64-v8a (A64) and armeabi-v7a (Thumb-2). Android 32-bit game code
// is Thumb-2 in practice, which is what the A32 path decodes. The address handed
// back is always the plain (even) function start - AndInlineHook's
// and_hook_detect_thumb() reads the prologue and sets bit0 for Dobby itself.
//
// Strategy A - call-site voting (preferred, both arches).
//   Py_file_input is 257, so a caller of PyRun_StringFlags must materialise
//   `257` into w1/r1 and then branch to it. Collect those branch targets and take
//   the most frequent one: the game's own call sites and CPython's internal
//   PyRun_SimpleStringFlags all agree on the same address, so noise loses.
//
// Strategy B - "<string>" cross-reference (fallback).
//   PyRun_StringFlags is the CPython function that passes the literal "<string>"
//   as the source filename. Find that literal, find the code that addresses it,
//   then walk back to the function prologue.
//
// Both read the *loaded* image, so a packer that decrypts .text at runtime does
// not defeat the scan - only on-disk bytes are encrypted.

#include <android/log.h>
#include <cstdint>
#include <cstring>
#include <map>
#include <vector>

#include "../../KittyMemory/KittyMemory.h"

#ifndef NX_PY_TAG
#define NX_PY_TAG "NexoraPy"
#endif
// main.cpp's LOGI/LOGE are compiled out; these always reach logcat.
#define NX_LOGI(...) __android_log_print(ANDROID_LOG_INFO,  NX_PY_TAG, __VA_ARGS__)
#define NX_LOGE(...) __android_log_print(ANDROID_LOG_ERROR, NX_PY_TAG, __VA_ARGS__)

namespace nx {

namespace detail {

struct Range { uintptr_t begin; uintptr_t end; };

inline std::vector<Range> execRanges(const std::vector<KittyMemory::ProcMap>& maps) {
    std::vector<Range> out;
    for (const auto& m : maps) {
        if (m.readable && m.executable && m.length >= 4)
            out.push_back({ (uintptr_t)m.startAddress, (uintptr_t)m.endAddress });
    }
    return out;
}

inline uintptr_t bestVote(const std::map<uintptr_t, int>& votes, int* votesOut) {
    uintptr_t best = 0;
    int bestVotes = 0;
    for (const auto& kv : votes) {
        if (kv.second > bestVotes) { bestVotes = kv.second; best = kv.first; }
    }
    if (votesOut) *votesOut = bestVotes;
    return best;
}

// Every readable occurrence of `needle` (NUL included), capped for sanity.
inline std::vector<uintptr_t> findNeedle(const std::vector<KittyMemory::ProcMap>& maps,
                                         const char* needle, size_t maxHits) {
    std::vector<uintptr_t> hits;
    const size_t len = strlen(needle) + 1;
    for (const auto& m : maps) {
        if (!m.readable || m.length < len) continue;
        const char* cur = (const char*)(uintptr_t)m.startAddress;
        const char* end = cur + m.length;
        while (cur + len <= end) {
            const char* f = (const char*)memchr(cur, needle[0], (size_t)(end - cur));
            if (!f) break;
            if ((size_t)(end - f) >= len && memcmp(f, needle, len) == 0) {
                hits.push_back((uintptr_t)f);
                if (hits.size() >= maxHits) return hits;
            }
            cur = f + 1;
        }
    }
    return hits;
}

// The executable range containing `addr`, or 0.
inline uintptr_t rangeBeginOf(const std::vector<Range>& text, uintptr_t addr) {
    for (const auto& r : text) {
        if (addr >= r.begin && addr < r.end) return r.begin;
    }
    return 0;
}

} // namespace detail

#if defined(__aarch64__)
// ===========================================================================
//  arm64-v8a (A64)
// ===========================================================================
namespace detail {

// BL <imm26> -> absolute target; 0 when the instruction is not a BL.
inline uintptr_t blTargetA64(uintptr_t pc, uint32_t insn) {
    if ((insn & 0xFC000000u) != 0x94000000u) return 0;
    int32_t imm = (int32_t)(insn & 0x03FFFFFFu);
    if (imm & (1 << 25)) imm -= (1 << 26);
    return (uintptr_t)((int64_t)pc + ((int64_t)imm << 2));
}

// `movz w1, #0x101` == Py_file_input (257) in the second argument.
constexpr uint32_t kMovW1FileInput = 0x52802021u;

inline uintptr_t byCallSitesA64(const std::vector<Range>& text, int* votesOut) {
    std::map<uintptr_t, int> votes;
    for (const auto& r : text) {
        for (uintptr_t p = r.begin; p + 4 <= r.end; p += 4) {
            if (*(const uint32_t*)p != kMovW1FileInput) continue;
            // The call follows the argument setup within a handful of insns.
            for (int k = 1; k <= 8; ++k) {
                const uintptr_t q = p + (uintptr_t)k * 4;
                if (q + 4 > r.end) break;
                const uintptr_t t = blTargetA64(q, *(const uint32_t*)q);
                if (t) { votes[t]++; break; }
            }
        }
    }
    return bestVote(votes, votesOut);
}

// First ADRP+ADD pair resolving to `target`; returns the ADRP site.
inline uintptr_t xrefToA64(const std::vector<Range>& text, uintptr_t target) {
    for (const auto& r : text) {
        for (uintptr_t p = r.begin; p + 8 <= r.end; p += 4) {
            const uint32_t insn = *(const uint32_t*)p;
            if ((insn & 0x9F000000u) != 0x90000000u) continue;   // ADRP
            const uint32_t rd = insn & 0x1Fu;
            int64_t imm = (int64_t)((((insn >> 5) & 0x7FFFFu) << 2) | ((insn >> 29) & 0x3u));
            if (imm & (1LL << 20)) imm -= (1LL << 21);           // sign-extend 21 bits
            const uintptr_t page = (p & ~(uintptr_t)0xFFFu) + (uintptr_t)(imm << 12);
            for (int k = 1; k <= 3; ++k) {
                const uintptr_t q = p + (uintptr_t)k * 4;
                if (q + 4 > r.end) break;
                const uint32_t ins2 = *(const uint32_t*)q;
                if ((ins2 & 0xFF800000u) != 0x91000000u) continue; // add x, x, #imm (sh=0)
                if (((ins2 >> 5) & 0x1Fu) != rd) continue;         // Rn must be the ADRP dest
                if (page + ((ins2 >> 10) & 0xFFFu) == target) return p;
            }
        }
    }
    return 0;
}

// Walk back to `stp x29, x30, [sp, #-imm]!`, the frame-setup prologue.
inline uintptr_t functionStartA64(uintptr_t xref, uintptr_t textBegin, size_t maxBack) {
    uintptr_t limit = (xref > textBegin + maxBack) ? (xref - maxBack) : textBegin;
    limit = (limit + 3u) & ~(uintptr_t)3u;
    const uintptr_t from = xref & ~(uintptr_t)3u;
    for (uintptr_t p = from; p >= limit; p -= 4) {
        if ((*(const uint32_t*)p & 0xFFC00000u) == 0xA9800000u) return p;
        if (p == limit) break;
    }
    return 0;
}

} // namespace detail

#elif defined(__arm__)
// ===========================================================================
//  armeabi-v7a (Thumb-2)
// ===========================================================================
namespace detail {

// Thumb-2 marks a 32-bit instruction by opcode bits [15:11] == 11101/11110/11111.
inline bool thumb32(uint16_t hw) {
    const uint16_t top = hw >> 11;
    return top == 0x1D || top == 0x1E || top == 0x1F;
}

// BL (T1): 11110 S imm10 / 11 J1 1 J2 imm11. Target = PC + imm32, PC = addr + 4.
inline uintptr_t blTargetThumb(uintptr_t addr, uint16_t hw1, uint16_t hw2) {
    if ((hw1 & 0xF800) != 0xF000) return 0;   // 11110
    if ((hw2 & 0xD000) != 0xD000) return 0;   // 11 J1 1 -> BL (not BLX)

    const uint32_t S     = (hw1 >> 10) & 1u;
    const uint32_t imm10 = hw1 & 0x3FFu;
    const uint32_t J1    = (hw2 >> 13) & 1u;
    const uint32_t J2    = (hw2 >> 11) & 1u;
    const uint32_t imm11 = hw2 & 0x7FFu;
    const uint32_t I1    = (J1 ^ S) ^ 1u;
    const uint32_t I2    = (J2 ^ S) ^ 1u;

    uint32_t imm32 = (S << 24) | (I1 << 23) | (I2 << 22) | (imm10 << 12) | (imm11 << 1);
    if (S) imm32 |= 0xFE000000u;              // sign-extend
    // 32-bit addresses wrap naturally, so unsigned math is correct here.
    return (uintptr_t)(((uint32_t)addr + 4u) + imm32);
}

// MOVW (T3): 11110 i 10 0100 imm4 / 0 imm3 Rd imm8.
inline bool decodeMovw(uint16_t hw1, uint16_t hw2, uint32_t* rd, uint32_t* imm16) {
    if ((hw1 & 0xFBF0) != 0xF240) return false;
    if (hw2 & 0x8000) return false;
    const uint32_t imm4 = hw1 & 0xFu;
    const uint32_t i    = (hw1 >> 10) & 1u;
    const uint32_t imm3 = (hw2 >> 12) & 0x7u;
    const uint32_t r    = (hw2 >> 8) & 0xFu;
    const uint32_t imm8 = hw2 & 0xFFu;
    if (rd) *rd = r;
    if (imm16) *imm16 = (imm4 << 12) | (i << 11) | (imm3 << 8) | imm8;
    return true;
}

// MOVT (T1): 11110 i 10 1100 imm4 / 0 imm3 Rd imm8.
inline bool decodeMovt(uint16_t hw1, uint16_t hw2, uint32_t* rd, uint32_t* imm16) {
    if ((hw1 & 0xFBF0) != 0xF2C0) return false;
    if (hw2 & 0x8000) return false;
    const uint32_t imm4 = hw1 & 0xFu;
    const uint32_t i    = (hw1 >> 10) & 1u;
    const uint32_t imm3 = (hw2 >> 12) & 0x7u;
    const uint32_t r    = (hw2 >> 8) & 0xFu;
    const uint32_t imm8 = hw2 & 0xFFu;
    if (rd) *rd = r;
    if (imm16) *imm16 = (imm4 << 12) | (i << 11) | (imm3 << 8) | imm8;
    return true;
}

inline uintptr_t byCallSitesThumb(const std::vector<Range>& text, int* votesOut) {
    std::map<uintptr_t, int> votes;
    for (const auto& r : text) {
        for (uintptr_t p = r.begin; p + 4 <= r.end; p += 2) {
            const uint16_t hw1 = *(const uint16_t*)p;
            uint32_t rd = 0, imm16 = 0;
            if (!decodeMovw(hw1, *(const uint16_t*)(p + 2), &rd, &imm16)) continue;
            if (rd != 1) continue;                                   // start code -> r1
            if (imm16 != 256 && imm16 != 257 && imm16 != 258) continue; // single/file/eval

            // Step forward properly: 32-bit Thumb instructions are 4 bytes wide.
            uintptr_t q = p + 4;
            for (int step = 0; step < 12 && q + 4 <= r.end; ++step) {
                const uint16_t a = *(const uint16_t*)q;
                if (thumb32(a)) {
                    const uintptr_t t = blTargetThumb(q, a, *(const uint16_t*)(q + 2));
                    if (t) { votes[t]++; break; }
                    q += 4;
                } else {
                    q += 2;
                }
            }
        }
    }
    return bestVote(votes, votesOut);
}

// Code that forms `target` with a MOVW/MOVT pair (absolute addressing).
inline uintptr_t xrefToThumb(const std::vector<Range>& text, uintptr_t target) {
    for (const auto& r : text) {
        for (uintptr_t p = r.begin; p + 8 <= r.end; p += 2) {
            uint32_t rd1 = 0, lo = 0;
            if (!decodeMovw(*(const uint16_t*)p, *(const uint16_t*)(p + 2), &rd1, &lo)) continue;
            const uintptr_t q = p + 4;
            uint32_t rd2 = 0, hi = 0;
            if (!decodeMovt(*(const uint16_t*)q, *(const uint16_t*)(q + 2), &rd2, &hi)) continue;
            if (rd1 != rd2) continue;
            if (((hi << 16) | lo) == (uint32_t)target) return p;
        }
    }
    return 0;
}

// Walk back to `push {..., lr}` (PUSH T1 with the M bit set == 0xB5xx).
inline uintptr_t functionStartThumb(uintptr_t xref, uintptr_t textBegin, size_t maxBack) {
    uintptr_t limit = (xref > textBegin + maxBack) ? (xref - maxBack) : textBegin;
    limit &= ~(uintptr_t)1u;
    for (uintptr_t p = xref & ~(uintptr_t)1u; p >= limit; p -= 2) {
        const uint16_t hw = *(const uint16_t*)p;
        if ((hw & 0xFF00) == 0xB500) return p;   // push {..., lr}
        if (p == limit) break;
    }
    return 0;
}

} // namespace detail

#endif

/**
 * Locate the game's PyRun_StringFlags in a loaded module.
 * @param module  name as it appears in /proc/self/maps, e.g. "libGame.so"
 * @param verbose log the strategy and address that won
 * @return absolute (even) address of the function, or 0 when nothing was found
 */
inline uintptr_t findPyRunStringFlags(const char* module, bool verbose = true) {
#if !defined(__aarch64__) && !defined(__arm__)
    (void)module; (void)verbose;
    NX_LOGE("pattern scan supports arm64/arm only");
    return 0;
#else
    const auto maps = KittyMemory::getMapsByName(module);
    if (maps.empty()) { NX_LOGE("%s: not mapped", module); return 0; }

    const auto text = detail::execRanges(maps);
    if (text.empty()) { NX_LOGE("%s: no executable range", module); return 0; }

    int votes = 0;
#  if defined(__aarch64__)
    const uintptr_t byCalls = detail::byCallSitesA64(text, &votes);
    const char* arch = "arm64";
#  else
    const uintptr_t byCalls = detail::byCallSitesThumb(text, &votes);
    const char* arch = "arm32/thumb2";
#  endif

    if (byCalls && votes >= 2) {
        if (verbose)
            NX_LOGI("%s [%s]: PyRun_StringFlags = %p (call-site vote, %d hits)",
                    module, arch, (void*)byCalls, votes);
        return byCalls;
    }

    const auto strings = detail::findNeedle(maps, "<string>", 8);
    for (const uintptr_t s : strings) {
        uintptr_t xref = 0;
#  if defined(__aarch64__)
        xref = detail::xrefToA64(text, s);
#  else
        xref = detail::xrefToThumb(text, s);
#  endif
        if (!xref) continue;

        const uintptr_t rangeBegin = detail::rangeBeginOf(text, xref);
        if (!rangeBegin) continue;

#  if defined(__aarch64__)
        const uintptr_t fn = detail::functionStartA64(xref, rangeBegin, 0x800);
#  else
        const uintptr_t fn = detail::functionStartThumb(xref, rangeBegin, 0x800);
#  endif
        if (fn) {
            if (verbose)
                NX_LOGI("%s [%s]: PyRun_StringFlags = %p (\"<string>\" xref)",
                        module, arch, (void*)fn);
            return fn;
        }
    }

    if (byCalls) {
        if (verbose)
            NX_LOGI("%s [%s]: PyRun_StringFlags = %p (weak call-site vote, %d hit)",
                    module, arch, (void*)byCalls, votes);
        return byCalls;
    }

    NX_LOGE("%s [%s]: PyRun_StringFlags not found", module, arch);
    return 0;
#endif
}

} // namespace nx
