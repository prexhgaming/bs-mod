cmake_minimum_required(VERSION 3.22.1)
project("bs_dumper")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(THIRD_PARTY ${CMAKE_CURRENT_SOURCE_DIR}/third_party)

# ---------------------------------------------------------------------------
# xDL — MIT, github.com/hexhacking/xDL
# Enhanced dlopen/dlsym/dl_iterate_phdr. Used to resolve the game's symbols
# without touching the linker's private internals.
# ---------------------------------------------------------------------------
add_library(xdl STATIC
    ${THIRD_PARTY}/xdl/xdl.c
    ${THIRD_PARTY}/xdl/xdl_iterate.c
    ${THIRD_PARTY}/xdl/xdl_linker.c
    ${THIRD_PARTY}/xdl/xdl_lzma.c
    ${THIRD_PARTY}/xdl/xdl_util.c
)
target_include_directories(xdl PUBLIC ${THIRD_PARTY})
target_link_libraries(xdl PUBLIC dl log)

# ---------------------------------------------------------------------------
# Dobby — Apache-2.0, github.com/jmpews/Dobby
# Prebuilt static libs, one per ABI we ship.
# ---------------------------------------------------------------------------
add_library(dobby STATIC IMPORTED)
set_target_properties(dobby PROPERTIES
    IMPORTED_LOCATION ${THIRD_PARTY}/dobby/${ANDROID_ABI}/libdobby.a
)
target_include_directories(dobby INTERFACE ${THIRD_PARTY})

# ---------------------------------------------------------------------------
# KittyMemory — MIT, github.com/MJx0/KittyMemory
# Memory read/write, patching, pattern scanning. Keystone (the inline
# assembler used by MemoryPatch::createWithAsm) is deliberately disabled via
# kNO_KEYSTONE: it would add ~18 MB of prebuilt archives for a feature the
# dumper does not use.
# ---------------------------------------------------------------------------
add_library(kitty STATIC
    ${THIRD_PARTY}/kitty/KittyAsm.cpp
    ${THIRD_PARTY}/kitty/KittyIOFile.cpp
    ${THIRD_PARTY}/kitty/KittyMemory.cpp
    ${THIRD_PARTY}/kitty/KittyPtrValidator.cpp
    ${THIRD_PARTY}/kitty/KittyScanner.cpp
    ${THIRD_PARTY}/kitty/KittyUtils.cpp
    ${THIRD_PARTY}/kitty/MemoryBackup.cpp
    ${THIRD_PARTY}/kitty/MemoryPatch.cpp
)
target_include_directories(kitty PUBLIC ${THIRD_PARTY})
target_compile_definitions(kitty PUBLIC kNO_KEYSTONE)
target_link_libraries(kitty PUBLIC dl log)

# ---------------------------------------------------------------------------
# The dumper itself.
# ---------------------------------------------------------------------------
add_library(
    bs_dumper
    SHARED
    sdk_dumper.cpp
    native_utils.cpp
)

find_library(log-lib log)

target_include_directories(bs_dumper PRIVATE ${THIRD_PARTY})

target_link_libraries(
    bs_dumper
    xdl
    dobby
    kitty
    ${log-lib}
    dl
)
