//
//  MemoryPatch.cpp
//
//  Created by MJ (Ruit) on 1/1/19.
//

#include "MemoryPatch.h"
#include <sstream>
#include <iomanip>
#include <cstring>
#include <cstdlib>

MemoryPatch::MemoryPatch()
    : _address(0), _size(0)
{
}

MemoryPatch::MemoryPatch(const ProcMap &map, uintptr_t address,
                         const void *patch_code, size_t patch_size)
{
    if (!map.isValid() || patch_code == nullptr || patch_size == 0)
    {
        _address = 0;
        _size = 0;
        return;
    }

    _address = map.startAddress + address;
    _size = patch_size;

    _orig_code.resize(_size);
    _patch_code.resize(_size);

    // Read original bytes
    KittyMemory::memRead(_orig_code.data(), (void *)_address, _size);
    
    // Set patch bytes
    memcpy(_patch_code.data(), patch_code, _size);
}

MemoryPatch::MemoryPatch(uintptr_t absolute_address,
                         const void *patch_code, size_t patch_size)
{
    if (absolute_address == 0 || patch_code == nullptr || patch_size == 0)
    {
        _address = 0;
        _size = 0;
        return;
    }

    _address = absolute_address;
    _size = patch_size;

    _orig_code.resize(_size);
    _patch_code.resize(_size);

    // Read original bytes
    KittyMemory::memRead(_orig_code.data(), (void *)_address, _size);
    
    // Set patch bytes
    memcpy(_patch_code.data(), patch_code, _size);
}

MemoryPatch::~MemoryPatch()
{
}

// ====================================================================
// createWithHex - DIPERBAIKI
// ====================================================================
MemoryPatch MemoryPatch::createWithHex(const ProcMap &map, uintptr_t address, std::string hex)
{
    if (!map.isValid() || hex.empty())
        return MemoryPatch();

    // Step 1: Bersihkan string dari spasi, tab, newline
    std::string cleanHex;
    for (char c : hex) {
        if (c == ' ' || c == '\t' || c == '\n' || c == '\r')
            continue;
        cleanHex += c;
    }
    
    // Step 2: Hapus prefix 0x/0X
    if (cleanHex.length() >= 2 && 
        cleanHex[0] == '0' && (cleanHex[1] == 'x' || cleanHex[1] == 'X')) {
        cleanHex = cleanHex.substr(2);
    }

    // Step 3: Cek valid - harus genap
    if (cleanHex.length() < 2 || cleanHex.length() % 2 != 0) {
        KITTY_LOGE("createWithHex: Invalid hex length: %s", hex.c_str());
        return MemoryPatch();
    }

    // Step 4: Convert hex string ke bytes
    std::vector<uint8_t> bytes;
    for (size_t i = 0; i < cleanHex.length(); i += 2) {
        std::string byteStr = cleanHex.substr(i, 2);
        char* endptr;
        uint8_t byte = (uint8_t)strtoul(byteStr.c_str(), &endptr, 16);
        if (*endptr != '\0') {
            KITTY_LOGE("createWithHex: Invalid hex character: %s", byteStr.c_str());
            return MemoryPatch();
        }
        bytes.push_back(byte);
    }

    return MemoryPatch(map, address, bytes.data(), bytes.size());
}

MemoryPatch MemoryPatch::createWithHex(uintptr_t absolute_address, std::string hex)
{
    if (absolute_address == 0 || hex.empty())
        return MemoryPatch();

    // Step 1: Bersihkan string dari spasi, tab, newline
    std::string cleanHex;
    for (char c : hex) {
        if (c == ' ' || c == '\t' || c == '\n' || c == '\r')
            continue;
        cleanHex += c;
    }
    
    // Step 2: Hapus prefix 0x/0X
    if (cleanHex.length() >= 2 && 
        cleanHex[0] == '0' && (cleanHex[1] == 'x' || cleanHex[1] == 'X')) {
        cleanHex = cleanHex.substr(2);
    }

    // Step 3: Cek valid - harus genap
    if (cleanHex.length() < 2 || cleanHex.length() % 2 != 0) {
        KITTY_LOGE("createWithHex: Invalid hex length: %s", hex.c_str());
        return MemoryPatch();
    }

    // Step 4: Convert hex string ke bytes
    std::vector<uint8_t> bytes;
    for (size_t i = 0; i < cleanHex.length(); i += 2) {
        std::string byteStr = cleanHex.substr(i, 2);
        char* endptr;
        uint8_t byte = (uint8_t)strtoul(byteStr.c_str(), &endptr, 16);
        if (*endptr != '\0') {
            KITTY_LOGE("createWithHex: Invalid hex character: %s", byteStr.c_str());
            return MemoryPatch();
        }
        bytes.push_back(byte);
    }

    return MemoryPatch(absolute_address, bytes.data(), bytes.size());
}

bool MemoryPatch::isValid() const
{
    return (_address != 0 && _size > 0 && !_patch_code.empty());
}

size_t MemoryPatch::get_PatchSize() const
{
    return _size;
}

uintptr_t MemoryPatch::get_TargetAddress() const
{
    return _address;
}

bool MemoryPatch::Restore()
{
    if (!isValid())
        return false;
    
    if (_orig_code.empty())
        return false;
    
    return KittyMemory::memWrite((void *)_address, _orig_code.data(), _size);
}

bool MemoryPatch::Modify()
{
    if (!isValid())
        return false;
    
    if (_patch_code.empty())
        return false;
    
    return KittyMemory::memWrite((void *)_address, _patch_code.data(), _size);
}

std::string MemoryPatch::get_CurrBytes() const
{
    if (!isValid())
        return "";
    
    return KittyMemory::read2HexStr((void *)_address, _size);
}

std::string MemoryPatch::get_OrigBytes() const
{
    if (_orig_code.empty())
        return "";
    
    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    for (size_t i = 0; i < _orig_code.size(); i++)
    {
        ss << std::setw(2) << (int)_orig_code[i];
        if (i < _orig_code.size() - 1)
            ss << " ";
    }
    return ss.str();
}

std::string MemoryPatch::get_PatchBytes() const
{
    if (_patch_code.empty())
        return "";
    
    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    for (size_t i = 0; i < _patch_code.size(); i++)
    {
        ss << std::setw(2) << (int)_patch_code[i];
        if (i < _patch_code.size() - 1)
            ss << " ";
    }
    return ss.str();
}