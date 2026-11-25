//
//  KittyMemory.cpp
//
//  Created by MJ (Ruit) on 1/1/19.
//

#include "KittyMemory.h"
#include <fstream>
#include <sstream>
#include <cstring>
#include <iomanip>
#include <unistd.h>
#include <sys/mman.h>

namespace KittyMemory
{

    int setAddressProtection(void *address, size_t length, int protection)
    {
        uintptr_t pageStart = _PAGE_START_OF_(address);
        size_t pageLen = _PAGE_LEN_OF_(address, length);
        return mprotect((void *)pageStart, pageLen, protection);
    }

    bool memWrite(void *address, const void *buffer, size_t len)
    {
        if (address == nullptr || buffer == nullptr || len == 0)
            return false;

        uintptr_t pageStart = _PAGE_START_OF_(address);
        size_t pageLen = _PAGE_LEN_OF_(address, len);

        // Android 15 W^X fix:
        // Tidak boleh set PROT_WRITE + PROT_EXEC sekaligus -> SIGKILL
        // Pisah jadi 2 step: WRITE dulu -> copy -> restore EXEC
        if (mprotect((void *)pageStart, pageLen, PROT_READ | PROT_WRITE) == -1) {
            KITTY_LOGE("mprotect WRITE failed: %s", strerror(errno));
            return false;
        }

        memcpy(address, buffer, len);

        // Restore ke PROT_READ | PROT_EXEC (bukan RWX)
        if (mprotect((void *)pageStart, pageLen, PROT_READ | PROT_EXEC) == -1) {
            KITTY_LOGE("mprotect EXEC failed: %s", strerror(errno));
            // Coba restore ke READ saja
            mprotect((void *)pageStart, pageLen, PROT_READ);
            return false;
        }

        return true;
    }

    bool memRead(void *buffer, const void *address, size_t len)
    {
        if (address == nullptr || buffer == nullptr || len == 0)
            return false;

        memcpy(buffer, address, len);
        return true;
    }

    std::string read2HexStr(const void *address, size_t len)
    {
        if (address == nullptr || len == 0)
            return "";

        std::stringstream ss;
        ss << std::hex << std::setfill('0');
        
        unsigned char *bytes = (unsigned char *)address;
        for (size_t i = 0; i < len; i++)
        {
            ss << std::setw(2) << (int)bytes[i];
            if (i < len - 1)
                ss << " ";
        }
        
        return ss.str();
    }

    std::vector<ProcMap> getAllMaps()
    {
        std::vector<ProcMap> maps;
        std::ifstream ifs("/proc/self/maps");
        
        if (!ifs.is_open())
            return maps;

        std::string line;
        while (std::getline(ifs, line))
        {
            ProcMap map;
            char perms[5] = {0};
            char dev[12] = {0};
            char path[256] = {0};

            int matched = sscanf(line.c_str(), "%llx-%llx %4s %llx %11s %lu %255s",
                                 &map.startAddress, &map.endAddress, perms,
                                 &map.offset, dev, &map.inode, path);

            if (matched < 6)
                continue;

            map.length = map.endAddress - map.startAddress;
            
            map.readable   = (perms[0] == 'r');
            map.writeable  = (perms[1] == 'w');
            map.executable = (perms[2] == 'x');
            map.is_private = (perms[3] == 'p');
            map.is_shared  = (perms[3] == 's');
            
            map.protection = 0;
            if (map.readable)   map.protection |= PROT_READ;
            if (map.writeable)  map.protection |= PROT_WRITE;
            if (map.executable) map.protection |= PROT_EXEC;
            
            map.is_ro = map.readable && !map.writeable;
            map.is_rw = map.readable &&  map.writeable;
            map.is_rx = map.readable &&  map.executable;
            
            map.dev      = std::string(dev);
            map.pathname = std::string(path);
            
            maps.push_back(map);
        }
        
        ifs.close();
        return maps;
    }

    std::vector<ProcMap> getMapsByName(const std::string& name)
    {
        std::vector<ProcMap> result;
        auto maps = getAllMaps();
        
        for (auto& map : maps)
        {
            if (map.pathname.find(name) != std::string::npos)
                result.push_back(map);
        }
        
        return result;
    }

    ProcMap getAddressMap(const void *address)
    {
        auto maps = getAllMaps();
        uintptr_t addr = (uintptr_t)address;
        
        for (auto& map : maps)
        {
            if (addr >= map.startAddress && addr < map.endAddress)
                return map;
        }
        
        return ProcMap();
    }

    ProcMap getLibraryBaseMap(const std::vector<ProcMap>& maps)
    {
        for (auto& map : maps)
        {
            if (map.executable && map.readable)
                return map;
        }
        return ProcMap();
    }

    ProcMap getLibraryBaseMap(const std::string& name)
    {
        auto maps = getMapsByName(name);
        return getLibraryBaseMap(maps);
    }

}