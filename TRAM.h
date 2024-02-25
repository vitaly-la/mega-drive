#pragma once

struct TRAM : TDevice {
    TRAM(uint32_t offset, uint32_t size) : TDevice{offset} {
        Memory.resize(size);
    }
};
