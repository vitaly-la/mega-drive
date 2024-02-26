#pragma once

struct TRAM : TDevice {
    TRAM(u32 offset, u32 size) : TDevice{offset} {
        Memory.resize(size);
    }
};
