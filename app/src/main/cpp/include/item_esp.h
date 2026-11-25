#pragma once

#include <string>
#include <vector>
#include <thread>
#include <atomic>
#include <mutex>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/poll.h>
#include <cstring>
#include <cerrno>
#include <chrono>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

#ifndef PACKAGE_NAME
#define PACKAGE_NAME "com.netease.newspike"
#endif

// PATH HARUS SAMA PERSIS DENGAN PYTHON - LOOT_ESP BUKAN ITEMS!
const char* ITEM_FIFO_PATH = "/data/data/" PACKAGE_NAME "/imgui/loot_esp";

struct ItemESP {
    std::string name;
    float x, y, z;           // Screen position
    float distance;          // Distance from player
    int quality;             // Item quality (0-5)
    std::string type;        // Item type (Weapon, Airdrop, Box, Supply, ZombieDrop)
    float world_x, world_y, world_z; // World position
    
    bool IsValid() const {
        return !name.empty() && x > 0 && y > 0;
    }
    
    std::string GetDisplayText() const {
        if (name.empty()) return "";
        return name + " [" + std::to_string((int)distance) + "m]";
    }
    
    std::string GetDebugInfo() const {
        return "Name: " + name + 
               ", Type: " + type +
               ", Dist: " + std::to_string((int)distance) + "m" +
               ", Quality: " + std::to_string(quality) +
               ", Screen: (" + std::to_string((int)x) + ", " + std::to_string((int)y) + ")";
    }
};

class ItemESPReader {
private:
    std::thread reader_thread;
    std::mutex items_mutex;
    std::vector<ItemESP> items;
    std::atomic<bool> running{false};
    std::atomic<bool> enabled{true};
    std::string fifo_path;
    std::atomic<int> item_count{0};

    void ReaderFunction() {
        int fd = -1;
        std::vector<char> buffer(128 * 1024); // 128KB buffer
        size_t offset = 0;
        
        while (running.load()) {
            // Open FIFO if not open
            if (fd < 0) {
                struct stat st;
                if (stat(fifo_path.c_str(), &st) == -1) {
                    // FIFO doesn't exist yet, wait and retry
                    std::this_thread::sleep_for(std::chrono::milliseconds(500));
                    continue;
                }
                
                fd = open(fifo_path.c_str(), O_RDONLY | O_NONBLOCK);
                if (fd < 0) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(500));
                    continue;
                }
                
                offset = 0;
            }
            
            // Poll for data
            struct pollfd pfd = {fd, POLLIN, 0};
            int ret = poll(&pfd, 1, 100); // 100ms timeout
            
            if (ret < 0) {
                close(fd);
                fd = -1;
                continue;
            }
            
            if (pfd.revents & POLLIN) {
                ssize_t bytes = read(fd, buffer.data() + offset, buffer.size() - offset - 1);
                if (bytes <= 0) {
                    if (bytes < 0 && errno != EAGAIN) {
                        close(fd);
                        fd = -1;
                    }
                    continue;
                }
                
                offset += bytes;
                buffer[offset] = '\0';
                
                // Parse complete JSON lines
                char* start = buffer.data();
                char* end = strchr(start, '\n');
                
                while (end != nullptr) {
                    *end = '\0';
                    
                    try {
                        auto json_data = json::parse(start);
                        
                        if (json_data.is_array()) {
                            std::vector<ItemESP> new_items;
                            
                            for (const auto& item : json_data) {
                                ItemESP esp;
                                esp.name = item.value("name", "");
                                esp.x = item.value("x", 0.0f);
                                esp.y = item.value("y", 0.0f);
                                esp.z = item.value("z", 0.0f);
                                esp.distance = item.value("dist", 0.0f);
                                esp.quality = item.value("quality", 0);
                                esp.type = item.value("type", "Item");
                                esp.world_x = item.value("world_x", 0.0f);
                                esp.world_y = item.value("world_y", 0.0f);
                                esp.world_z = item.value("world_z", 0.0f);
                                
                                if (esp.IsValid()) {
                                    new_items.push_back(esp);
                                }
                            }
                            
                            {
                                std::lock_guard<std::mutex> lock(items_mutex);
                                items = std::move(new_items);
                                item_count.store(items.size());
                            }
                        }
                    } catch (const std::exception& e) {
                        // JSON parsing error, ignore
                    }
                    
                    start = end + 1;
                    end = strchr(start, '\n');
                }
                
                // Move remaining data to beginning of buffer
                if (start != buffer.data()) {
                    size_t remaining = offset - (start - buffer.data());
                    if (remaining > 0 && remaining < buffer.size()) {
                        memmove(buffer.data(), start, remaining);
                        offset = remaining;
                    } else {
                        offset = 0;
                    }
                }
            } else if (pfd.revents & (POLLERR | POLLHUP | POLLNVAL)) {
                close(fd);
                fd = -1;
            }
        }
        
        if (fd >= 0) close(fd);
    }

public:
    ItemESPReader(const char* path) : fifo_path(path) {
    }
    
    ~ItemESPReader() {
        Stop();
    }
    
    void Start() {
        if (running.exchange(true)) return;
        reader_thread = std::thread(&ItemESPReader::ReaderFunction, this);
        reader_thread.detach();
    }
    
    void Stop() {
        running.store(false);
    }
    
    std::vector<ItemESP> GetItems() {
        if (!enabled.load()) return {};
        std::lock_guard<std::mutex> lock(items_mutex);
        return items;
    }
    
    void SetEnabled(bool state) {
        enabled.store(state);
    }
    
    bool IsEnabled() const {
        return enabled.load();
    }
    
    int GetItemCount() const {
        return item_count.load();
    }
    
    void ClearItems() {
        std::lock_guard<std::mutex> lock(items_mutex);
        items.clear();
        item_count.store(0);
    }
    
    // Get items filtered by type
    std::vector<ItemESP> GetItemsByType(const std::string& type) {
        std::vector<ItemESP> filtered;
        std::lock_guard<std::mutex> lock(items_mutex);
        
        for (const auto& item : items) {
            if (item.type == type) {
                filtered.push_back(item);
            }
        }
        
        return filtered;
    }
    
    // Get weapons specifically
    std::vector<ItemESP> GetWeapons() {
        return GetItemsByType("Weapon");
    }
    
    // Get nearest item
    ItemESP GetNearestItem() {
        ItemESP nearest;
        float min_dist = 999999.0f;
        
        std::lock_guard<std::mutex> lock(items_mutex);
        for (const auto& item : items) {
            if (item.distance < min_dist) {
                min_dist = item.distance;
                nearest = item;
            }
        }
        
        return nearest;
    }
};

// Global instance - PASTIKAN PATH NYA BENAR!
static ItemESPReader g_ItemESP(ITEM_FIFO_PATH);