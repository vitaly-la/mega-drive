#pragma once

struct TROM : TDevice {
    explicit TROM(const std::string& binFile) : TDevice{0x0, true} {
        std::ifstream input{binFile, std::ios::binary};
        std::copy(
            std::istreambuf_iterator<char>{input},
            std::istreambuf_iterator<char>{},
            std::back_inserter(Memory)
        );
    }
};
