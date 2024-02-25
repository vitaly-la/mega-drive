#pragma once

#include <vector>

struct TDevice {
    uint32_t Offset;
    std::vector<uint8_t> Memory;

    explicit TDevice(uint32_t offset) : Offset{offset} {}

    bool HasAddr(uint32_t addr) const {
        return Offset <= addr && addr < Offset + Memory.size();
    }

    uint32_t Read32(uint32_t addr) const {
        return Memory[addr - Offset]     << 24 |
               Memory[addr - Offset + 1] << 16 |
               Memory[addr - Offset + 2] << 8  |
               Memory[addr - Offset + 3];
    }
};

struct TMemory {
    std::vector<std::unique_ptr<TDevice>> Devices;

    void Connect(std::unique_ptr<TDevice> device) {
        Devices.push_back(std::move(device));
    }

    uint32_t Read32(uint32_t addr) const {
        for (const auto& device : Devices) {
            if (device->HasAddr(addr)) {
                return device->Read32(addr);
            }
        }

        throw std::runtime_error("Segmentation fault.");
    }
};
