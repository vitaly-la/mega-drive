#pragma once

template<class T>
constexpr void CheckType() {
    static_assert(std::is_same<T, u8>::value ||
                  std::is_same<T, u16>::value ||
                  std::is_same<T, u32>::value,
                  "Wrong template type.");
}

struct TDevice {
    u32 Offset;
    std::vector<u8> Memory;
    bool WriteProtected;

    explicit TDevice(u32 offset, bool writeProtected = false) : Offset{offset}, WriteProtected{writeProtected} {}

    template<class T>
    bool HasAddr(u32 addr) const {
        CheckType<T>();

        for (size_t i = 0; i < sizeof(T) / sizeof(u8); ++i) {
            if (addr + i < Offset || addr + i >= Offset + Memory.size()) {
                return false;
            }
        }

        return true;
    }

    template<class T>
    T Read(u32 addr) const {
        CheckType<T>();

        if constexpr (std::is_same<T, u8>::value) {
            return Memory[addr - Offset];
        } else if (std::is_same<T, u16>::value) {
            return Memory[addr - Offset] << 8 |
                   Memory[addr - Offset + 1];
        } else {
            return Memory[addr - Offset]     << 24 |
                   Memory[addr - Offset + 1] << 16 |
                   Memory[addr - Offset + 2] << 8  |
                   Memory[addr - Offset + 3];
        }
    }

    template<class T>
    void Write(u32 addr, T value) {
        CheckType<T>();

        if (WriteProtected) {
            throw std::runtime_error("Device is write-protected.");
        }

        if constexpr (std::is_same<T, u8>::value) {
            Memory[addr - Offset] = value;
        } else if (std::is_same<T, u16>::value) {
            Memory[addr - Offset] = value >> 8;
            Memory[addr - Offset + 1] = value % (1 << 8);
        } else {
            Memory[addr - Offset] = value >> 24;
            Memory[addr - Offset + 1] = (value >> 16) % (1 << 8);
            Memory[addr - Offset + 2] = (value >> 8) % (1 << 8);
            Memory[addr - Offset + 3] = value % (1 << 8);
        }
    }
};

struct TMemory {
    std::vector<TDevice> Devices;

    void Connect(TDevice device) {
        Devices.push_back(std::move(device));
    }

    template<class T>
    T Read(u32 addr) const {
        for (const auto& device : Devices) {
            if (device.HasAddr<T>(addr)) {
                return device.Read<T>(addr);
            }
        }

        throw std::runtime_error("Segmentation fault.");
    }

    template<class T>
    void Write(u32 addr, T value) {
        for (auto& device : Devices) {
            if (device.HasAddr<T>(addr)) {
                device.Write<T>(addr, value);
                return;
            }
        }

        throw std::runtime_error("Segmentation fault.");
    }
};
