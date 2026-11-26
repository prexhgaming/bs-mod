/* ============================================================================
 * AndInlineHook/AndInlineHook.h — one hook API, one engine.
 *
 *   Dobby is the hooking engine on BOTH architectures:
 *     ARM32 (armeabi-v7a) -> Dobby/libraries/armeabi-v7a/libdobby.a
 *     ARM64 (arm64-v8a)   -> Dobby/libraries/arm64-v8a/libdobby.a
 *
 * The public surface is identical on both ABIs, so call sites and macros
 * (HOOKI / HOOKNI / HOOK / HOOKN / _AndHookF) never change between a 32-bit
 * and a 64-bit build.
 *
 * Use:
 *     AndHookFunction((void *)(libGameBase + 0x471E320), (void *)myFunc, (void **)&orig);
 *   or the legacy macros that existing code already uses:
 *     HOOKI((ptr)libGameBase + 0x471E320, iMissMyWife);   // declares _iMissMyWife
 *
 * ARM32 note: raw `base + offset` addresses carry no Thumb state bit, but
 * Dobby needs it to disambiguate ARM vs Thumb code. This header therefore
 * detects the state from the first instruction word (Android 32-bit game code
 * is Thumb-2 in practice) and ORs bit0 in for Dobby. Use AndHookForceThumb()
 * / AndHookForceArm() if a target ever needs to override the guess.
 *
 * Header-only: everything is `static` + unused-attributed, so it is safe to
 * include from more than one translation unit (each TU gets its own small
 * registry; the actual hooks live in Dobby and are global).
 * ==========================================================================*/
#ifndef AND_INLINE_HOOK_H
#define AND_INLINE_HOOK_H

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

#if defined(__GNUC__)
#  define AND_HOOK_UNUSED __attribute__((unused))
#else
#  define AND_HOOK_UNUSED
#endif

/* ------------------------------------------------------------------ arch -- */
#if !defined(AND_HOOK_ARCH_A32) && !defined(AND_HOOK_ARCH_A64) && !defined(AND_HOOK_ARCH_OTHER)
#  if defined(__aarch64__) || defined(_M_ARM64)
#    define AND_HOOK_ARCH_A64 1
#  elif defined(__arm__) || defined(_M_ARM)
#    define AND_HOOK_ARCH_A32 1
#  else
#    define AND_HOOK_ARCH_OTHER 1
#  endif
#endif

/* ----------------------------------------------------------------- Dobby -- */
#if defined(AND_HOOK_ARCH_A32) || defined(AND_HOOK_ARCH_A64)
#  if defined(__has_include)
#    if __has_include("Dobby/dobby.h")
#      include "Dobby/dobby.h"
#    elif __has_include("../Dobby/dobby.h")
#      include "../Dobby/dobby.h"
#    else
#      error "AndInlineHook.h: Dobby/dobby.h not found - add the jni root (and Dobby/) to LOCAL_C_INCLUDES"
#    endif
#  else
#    include "Dobby/dobby.h"
#  endif
#endif

/* ------------------------------------------------------------ return codes */
#define AND_HOOK_OK                  0
#define AND_HOOK_ERR_ARG            -1   /* null symbol / replacement        */
#define AND_HOOK_ERR_ARCH           -2   /* not an ARM target                */
#define AND_HOOK_ERR_BACKEND        -3   /* Dobby refused or failed          */
#define AND_HOOK_ERR_ALREADY_HOOKED -4
#define AND_HOOK_ERR_NOT_HOOKED     -5
#define AND_HOOK_ERR_REGISTRY_FULL  -6

#ifndef AND_HOOK_MAX_HOOKS
#  define AND_HOOK_MAX_HOOKS 128
#endif

/* ------------------------------------------------------------------ error */
AND_HOOK_UNUSED static const char *and_hook_error = "no error";

AND_HOOK_UNUSED static void and_hook_err(const char *msg) {
    static char storage[160];
    if (!msg) msg = "unknown error";
    strncpy(storage, msg, sizeof(storage) - 1);
    storage[sizeof(storage) - 1] = '\0';
    and_hook_error = storage;
}

AND_HOOK_UNUSED static const char *AndHookLastError(void) { return and_hook_error; }

/* --------------------------------------------------------------- registry */
typedef struct {
    void    *symbol;      /* aligned target address                     */
    uint8_t  used;
    uint8_t  thumb;       /* ARM32: 1 when bit0 was set for Dobby       */
} and_hook_rec_t;

AND_HOOK_UNUSED static and_hook_rec_t and_hook_records[AND_HOOK_MAX_HOOKS];
AND_HOOK_UNUSED static int and_hook_force_thumb = -1;   /* -1 = auto        */

AND_HOOK_UNUSED static and_hook_rec_t *and_hook_find(void *symbol) {
    uintptr_t want = (uintptr_t)symbol & ~(uintptr_t)1;
    for (int i = 0; i < AND_HOOK_MAX_HOOKS; i++) {
        if (and_hook_records[i].used &&
            ((uintptr_t)and_hook_records[i].symbol & ~(uintptr_t)1) == want) {
            return &and_hook_records[i];
        }
    }
    return NULL;
}

AND_HOOK_UNUSED static and_hook_rec_t *and_hook_alloc(void) {
    for (int i = 0; i < AND_HOOK_MAX_HOOKS; i++) {
        if (!and_hook_records[i].used) return &and_hook_records[i];
    }
    return NULL;
}

/* ------------------------------------------------------------ ARM32 state */
#if defined(AND_HOOK_ARCH_A32)
/* Returns 1 for Thumb, 0 for ARM. Never dereferences more than 4 bytes. */
AND_HOOK_UNUSED static int and_hook_detect_thumb(uintptr_t addr) {
    if (addr & 1u) return 1;                    /* caller told us explicitly */
    if (and_hook_force_thumb >= 0) return and_hook_force_thumb;

    uint16_t h0 = 0, h1 = 0;
    memcpy(&h0, (const void *)addr, 2);
    memcpy(&h1, (const void *)(addr + 2), 2);
    (void)h1;

    /* positive Thumb-2 prologue patterns */
    if ((h0 & 0xFE00) == 0xB400) return 1;      /* PUSH/POP {...,lr}        */
    if ((h0 & 0xFF80) == 0xB080) return 1;      /* SUB SP, SP, #imm         */
    if ((h0 & 0xF800) == 0x4800) return 1;      /* LDR Rd, [PC, #imm]       */
    if ((h0 & 0xFF00) == 0xBF00) return 1;      /* NOP / IT / hints         */
    if ((h0 & 0xF500) == 0xB100) return 1;      /* CBZ / CBNZ               */
    if ((h0 & 0xE000) == 0xE000) return 1;      /* any 32-bit Thumb insn    */

    /* clear ARM prologue patterns */
    if (h0 == 0xE92D) return 0;                 /* PUSH {.., lr}            */
    if ((h0 & 0xFFF0) == 0xE1A0) return 0;      /* MOV / shift              */
    if ((h0 & 0xFFF0) == 0xE24D) return 0;      /* SUB SP, SP, #imm         */
    if ((h0 & 0xFF00) == 0xE500) return 0;      /* STR/LDR                  */

    /* Android 32-bit game code is Thumb-2; that is the safe default. */
    return 1;
}
#endif

AND_HOOK_UNUSED static void AndHookForceThumb(int enable) { and_hook_force_thumb = enable ? 1 : 0; }
AND_HOOK_UNUSED static void AndHookForceArm(void)         { and_hook_force_thumb = 0; }
AND_HOOK_UNUSED static void AndHookStateAuto(void)        { and_hook_force_thumb = -1; }

AND_HOOK_UNUSED static int AndHookIsThumb(const void *symbol) {
#if defined(AND_HOOK_ARCH_A32)
    return and_hook_detect_thumb((uintptr_t)symbol);
#else
    (void)symbol;
    return 0;
#endif
}

/* --------------------------------------------------------------- backend -- */
AND_HOOK_UNUSED static const char *AndHookBackendName(void) {
#if defined(AND_HOOK_ARCH_A32)
    return "Dobby (arm32)";
#elif defined(AND_HOOK_ARCH_A64)
    return "Dobby (arm64)";
#else
    return "none";
#endif
}

AND_HOOK_UNUSED static int AndHookInit(void) {
    and_hook_err("ok");
    return AND_HOOK_OK;
}

/* ------------------------------------------------------------------ API --- */
/* Install an inline hook. 0 on success, negative AND_HOOK_ERR_* otherwise. */
AND_HOOK_UNUSED static int AndHookFunction(void *symbol, void *replace, void **backup) {
    if (!symbol || !replace) {
        and_hook_err("null symbol or replacement");
        return AND_HOOK_ERR_ARG;
    }
#if defined(AND_HOOK_ARCH_OTHER)
    (void)backup;
    and_hook_err("unsupported architecture");
    return AND_HOOK_ERR_ARCH;
#else
    uintptr_t addr = (uintptr_t)symbol;
    int thumb = 0;

#  if defined(AND_HOOK_ARCH_A32)
    thumb = and_hook_detect_thumb(addr);
    addr &= ~(uintptr_t)1;
    if (thumb) addr |= 1u;                  /* Dobby needs the state bit */
#  endif

    void *key = (void *)(addr & ~(uintptr_t)1);
    if (and_hook_find(key)) {
        and_hook_err("address is already hooked");
        return AND_HOOK_ERR_ALREADY_HOOKED;
    }

    and_hook_rec_t *rec = and_hook_alloc();
    if (!rec) {
        and_hook_err("hook registry full");
        return AND_HOOK_ERR_REGISTRY_FULL;
    }

    int rc = DobbyHook((void *)addr, (dobby_dummy_func_t)replace,
                       (dobby_dummy_func_t *)backup);
    if (rc != 0) {
        and_hook_err("DobbyHook refused this target");
        return AND_HOOK_ERR_BACKEND;
    }

    rec->used   = 1;
    rec->symbol = key;
    rec->thumb  = (uint8_t)thumb;
    and_hook_err("ok");
    return AND_HOOK_OK;
#endif
}

/* Same as AndHookFunction, kept for API compatibility with And64InlineHook.
 * Dobby owns its own trampoline memory, so rwx/rwx_size are unused: Dobby is
 * always free to place the trampoline where it can reach the target. */
AND_HOOK_UNUSED static void *AndHookFunctionV(void *symbol, void *replace,
                                              void *rwx, uintptr_t rwx_size) {
    (void)rwx;
    (void)rwx_size;
    if (AndHookFunction(symbol, replace, NULL) != AND_HOOK_OK) return NULL;
    return (void *)1;                      /* non-NULL == installed */
}

/* Restore the original instructions of a hooked address. */
AND_HOOK_UNUSED static int AndHookUnhook(void *symbol) {
    if (!symbol) {
        and_hook_err("null symbol");
        return AND_HOOK_ERR_ARG;
    }
    and_hook_rec_t *rec = and_hook_find(symbol);
    if (!rec) {
        and_hook_err("address is not hooked");
        return AND_HOOK_ERR_NOT_HOOKED;
    }
#if defined(AND_HOOK_ARCH_OTHER)
    and_hook_err("unsupported architecture");
    return AND_HOOK_ERR_ARCH;
#else
    int rc = DobbyDestroy((void *)((uintptr_t)rec->symbol & ~(uintptr_t)1));
    if (rc != 0) {
        and_hook_err("DobbyDestroy failed");
        return AND_HOOK_ERR_BACKEND;
    }
    rec->used   = 0;
    rec->symbol = NULL;
    and_hook_err("ok");
    return AND_HOOK_OK;
#endif
}

/* Internal helper the HOOK* macros expand to (same shape as _A64HookF). */
AND_HOOK_UNUSED static void _AndHookF(void *addr, void *func, void **backup) {
    (void)AndHookFunction(addr, func, backup);
}

/* ------------------------------------------------ legacy names / macros --- */
/* Old call sites keep working unchanged. */
#define A32HookFunction   AndHookFunction
#define A64HookFunction   AndHookFunction
#define A32HookFunctionV  AndHookFunctionV
#define A64HookFunctionV  AndHookFunctionV
#define A32HookInit       AndHookInit
#define A64HookInit       AndHookInit
#define _A64HookF         _AndHookF
#define _A32HookF         _AndHookF

#ifndef AND_HOOK_NO_COMPAT_MACROS

/* ---- argument counting + DEFINE(S): identical to the old And64InlineHook ---- */
#define GET_ARG_COUNT(...) INTERNAL_GET_ARG_COUNT_PRIVATE(0, ## __VA_ARGS__, 120, 119, 118, 117, 116, 115, 114, 113, 112, 111, 110, 109, 108, 107, 106, 105, 104, 103, 102, 101, 100, 99, 98, 97, 96, 95, 94, 93, 92, 91, 90, 89, 88, 87, 86, 85, 84, 83, 82, 81, 80, 79, 78, 77, 76, 75, 74, 73, 72, 71, 70, 69, 68, 67, 66, 65, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
#define INTERNAL_GET_ARG_COUNT_PRIVATE(_0, _1_, _2_, _3_, _4_, _5_, _6_, _7_, _8_, _9_, _10_, _11_, _12_, _13_, _14_, _15_, _16_, _17_, _18_, _19_, _20_, _21_, _22_, _23_, _24_, _25_, _26_, _27_, _28_, _29_, _30_, _31_, _32_, _33_, _34_, _35_, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, _51, _52, _53, _54, _55, _56, _57, _58, _59, _60, _61, _62, _63, _64, _65, _66, _67, _68, _69, _70, _71, _72, _73, _74, _75, _76, _77, _78, _79, _80, _81, _82, _83, _84, _85, _86, _87, _88, _89, _90, _91, _92, _93, _94, _95, _96, _97, _98, _99, _100, _101, _102, _103, _104, _105, _106, _107, _108, _109, _110, _111, _112, _113, _114, _115, _116, _117, _118, _119, _120, count, ...) count

/* Declares both the replacement `func` and its backup pointer `_func`. */
#define DEFINES(type, func, ...) int __##func = GET_ARG_COUNT(__VA_ARGS__); static type (*_##func)(__VA_ARGS__); static type func(__VA_ARGS__)
#define DEFINE(type, func, ...)  int __##func = GET_ARG_COUNT(__VA_ARGS__); type (*_##func)(ptr, ##__VA_ARGS__); type func(ptr inst, ##__VA_ARGS__)

/* ---- address based (raw absolute address, e.g. libGameBase + offset) ---- */
#  define HOOKI(addr, func)  _AndHookF((void *)(addr), (void *)func, (void **)&_##func)
#  define HOOKNI(addr, func) _AndHookF((void *)(addr), (void *)func, NULL)

/* ---- il2cpp based (needs `il2cpp` and `Method()` in scope at the call site) ---- */
#  ifndef AND_HOOK_NO_IL2CPP_MACROS
#    define HOOK(addr, func)       _AndHookF((void *)(il2cpp + (addr)), (void *)func, (void **)&_##func)
#    define HOOKN(addr, func)      _AndHookF((void *)(il2cpp + (addr)), (void *)func, NULL)
#    define HOOKM(a, b, c, func)   _AndHookF((void *)(Method(a, b, c, __##func)), (void *)func, (void **)&_##func); __##func = -1446
#    define HOOKNM(a, b, c, d, func) _AndHookF((void *)(Method(a, b, c, d)), (void *)func, NULL)
#    define H(a, b, c, d, func)    _AndHookF((void *)(Method(#a, #b, #c, d)), (void *)func, NULL)
#    define I(a, b, c, func)       _AndHookF((void *)(Method(#a, #b, #c, __##func)), (void *)func, (void **)&_##func); __##func = -1446
#  endif

/* ---- symbol based (fake_dlfcn helpers must be in scope at the call site) ---- */
#  define HOOKIT(library, symbol, func) do { \
    void *lib = fdlopen(library, 4); LOGI("lib %s: %p", library, lib); if (!lib) break; \
    void *p = fdlsym(lib, symbol); LOGI("sym %s: %p", symbol, p); fdlclose(lib); \
    if (p) HOOKI(p, func); } while (0)

#  define HOOKIT2(library, sym1, func1, sym2, func2) do { \
    void *lib = fdlopen(library, 4); LOGI("lib %s: %p", library, lib); if (!lib) break; \
    void *p1 = fdlsym(lib, sym1); LOGI("sym %s: %p", sym1, p1); \
    void *p2 = fdlsym(lib, sym2); LOGI("sym %s: %p", sym2, p2); fdlclose(lib); \
    if (p1 && p2) { HOOKI(p1, func1); HOOKI(p2, func2); } } while (0)

#endif /* AND_HOOK_NO_COMPAT_MACROS */

#endif /* AND_INLINE_HOOK_H */
