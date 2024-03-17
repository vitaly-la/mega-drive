#pragma once

template<size_t Size>
void Process(const std::array<int, Size>& mask, u16 opcode, std::function<void()> process) {
    auto bitset = std::bitset<sizeof(opcode)>(opcode);
    size_t i = 0;
    for (auto part : mask) {
        switch (part) {
        case 0:
            if (bitset[i] == 0) {
                ++i;
                continue;
            }
            return;
        case 1:
            if (bitset[i] == 1) {
                ++i;
                continue;
            }
            return;
        default:
            throw std::runtime_error("Unknown opcode mask.");
        }
    }
    process();
}
