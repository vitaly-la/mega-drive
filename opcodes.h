#pragma once

constexpr int S = 2;
constexpr int M = 3;
constexpr int Xn = 4;

enum struct ESize : u8 {
    Byte,
    Word,
    Long
};

enum struct EMode : u8 {
    Dn,
    An,
    Addr,
    AddrInc,
    AddrDec,
    AddrDisp,
    AddrIdx,
    Other
};

enum struct EModeExt : u8 {
    PCDisp = 2,
    PCIdx = 3,
    AbsShort = 0,
    AbsLong = 1,
    Imm = 4
};

struct TInstruction {
    ESize size;
    EMode mode;
    union {
        u8 reg;
        EModeExt modeExt;
    };
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

            case S: {
                u8 size = (bitset[i] << 1) | bitset[i + 1];
                if (size < 3) {
                    instruction.size = ESize{size};
                } else {
                    return false;
                }
                i += 2;
                continue;
            }

            case M: {
                u8 mode = (bitset[i] << 2) | (bitset[i + 1] << 1) | bitset[i + 2];
                if (mode < 8) {
                    instruction.mode = EMode{mode};
                } else {
                    return false;
                }
                i += 3;
                continue;
            }

            case Xn: {
                u8 reg = (bitset[i] << 2) | (bitset[i + 1] << 1) | bitset[i + 2];
                if (reg < 8) {
                    instruction.reg = reg;
                } else {
                    return false;
                }
                i += 3;
                continue;
            }

            default:
                throw std::runtime_error("Unknown opcode mask.");
        }
    }

    process(instruction);
    return true;
}
