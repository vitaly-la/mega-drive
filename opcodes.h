#pragma once

constexpr int S = 2;

enum struct ESize {
    Byte,
    Word,
    Long
};

struct TInstruction {
    ESize size;
};

template<size_t Size>
bool Process(const std::array<int, Size>& mask, u16 opcode, std::function<void(const TInstruction&)> process) {
    auto bitset = std::bitset<sizeof(opcode)>(opcode);
    size_t i = 0;
    TInstruction instruction;

    for (auto part : mask) {
        switch (part) {

        case 0:
            if (bitset[i] == 0) {
                ++i;
                continue;
            }
            return false;

        case 1:
            if (bitset[i] == 1) {
                ++i;
                continue;
            }
            return false;

        case S:
            if (bitset[i] == 0 && bitset[i + 1] == 0) {
                instruction.size = ESize::Byte;
            } else if (bitset[i] == 0 && bitset[i + 1] == 1) {
                instruction.size = ESize::Word;
            } else if (bitset[i] == 1 && bitset[i + 1] == 0) {
                instruction.size = ESize::Long;
            } else {
                return false;
            }
            i += 2;
            continue;

        default:
            throw std::runtime_error("Unknown opcode mask.");
        }
    }

    process(instruction);
    return true;
}
