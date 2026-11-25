// native_utils.hpp — small, self-contained helpers built on xDL, Dobby and
// KittyMemory.
//
// Deliberately not a copy of the mod-menu's `Includes/Utils.*`: that version
// defines globals inside a header (breaks with more than one translation unit)
// and pulls in Keystone through Macros.h. Everything here is declared once and
// defined in native_utils.cpp.

#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace bsu {

// ---------------------------------------------------------------- modules

/** Absolute path of the module containing [addr], or "" when unresolved. */
std::string modulePathForAddress(uintptr_t addr);

/** Load address (first mapping) of [module], or 0 when it is not loaded. */
uintptr_t libraryBase(const std::string& module);

/** True when [module] appears in /proc/self/maps. */
bool isLibraryLoaded(const std::string& module);

/** Every module path currently mapped into the process. */
std::vector<std::string> loadedModules();

// ---------------------------------------------------------------- symbols

/**
 * Address of [symbol] inside [module].
 *
 * Looks in .dynsym first, then .symtab / .gnu_debugdata. Returns 0 when the
 * symbol cannot be found.
 */
uintptr_t symbolAddress(const std::string& module, const std::string& symbol);

/**
 * Resolve either a hexadecimal offset ("0x1A2B3C" or "1a2b3c") relative to the
 * module base, or a symbol name, into an absolute address. Returns 0 on
 * failure.
 */
uintptr_t resolveAddress(const std::string& module, const std::string& offsetOrSymbol);

// ---------------------------------------------------------------- hooking

/**
 * Inline-hook [target] so calls land in [replacement].
 *
 * [original] receives a trampoline that runs the original code, or nullptr when
 * the caller does not need it. Returns true on success.
 */
bool hookFunction(uintptr_t target, void* replacement, void** original);

/** Convenience overload that resolves [offsetOrSymbol] inside [module] first. */
bool hookModule(const std::string& module, const std::string& offsetOrSymbol,
                void* replacement, void** original);

/** Remove a hook previously installed on [target]. */
bool unhookFunction(uintptr_t target);

// ---------------------------------------------------------------- patching

/** Overwrite [size] bytes at [target] with [bytes]. Returns true on success. */
bool patchBytes(uintptr_t target, const void* bytes, size_t size);

/** Restore the original bytes of a previous patchBytes() call. */
bool restorePatch(uintptr_t target);

/** Read [size] bytes from [target] into [out]. */
bool readBytes(uintptr_t target, void* out, size_t size);

}  // namespace bsu
