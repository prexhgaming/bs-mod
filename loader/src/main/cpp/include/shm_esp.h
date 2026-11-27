#pragma once
// shm_esp.h — Shared Memory layout untuk ESP data
// Python tulis, C++ baca. Zero copy, zero syscall per frame.

#include <stdint.h>

#define SHM_ESP_SIZE    (512 * 1024 + 16)  // 512KB data + 16 byte header
#define SHM_ESP_MAGIC   0xE5DA7A00U        // sanity check

#pragma pack(push, 1)
struct ShmEspHeader {
    uint32_t magic;         // offset 0  — SHM_ESP_MAGIC
    uint32_t data_size;     // offset 4  — ukuran JSON payload (bytes)
    uint64_t write_seq;     // offset 8  — increment tiap Python selesai nulis
    // offset 16: json payload mulai dari sini
};
#pragma pack(pop)

static_assert(sizeof(ShmEspHeader) == 16, "ShmEspHeader must be 16 bytes");
