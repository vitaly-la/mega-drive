#pragma once

#include <optional>
#include <string>

struct TArg;
struct TProcessor;

/*================ SYSTEM COMPONENTS ================*/

struct TInstruction {
    uint8_t size;
    std::string text;
    std::string cmd;
    std::vector<TArg> args;
};

void process_instruction(const TInstruction&, TProcessor&, std::vector<uint8_t>&);

struct TProcessor {
    uint32_t pc;
    uint16_t sr;
    uint32_t reg[16];

    explicit TProcessor(uint32_t pc) :
        pc{pc},
        sr{},
        reg{}
    {}

    void run(const TInstruction& instr, std::vector<uint8_t>& ram) {
        process_instruction(instr, *this, ram);
    }
};

/*================ ARGUMENT TYPES ================*/

enum struct ESize { B, W, L };

struct TArg {
    std::optional<uint8_t> direct;
    std::optional<uint8_t> indirect;
    std::optional<uint32_t> disp;
    std::optional<uint8_t> idx;
    std::optional<uint32_t> addr;
    std::optional<uint32_t> imm;
    bool inc;
    bool dec;
    bool pc;

    uint32_t Addr(const TProcessor& proc) const {
        if (addr) {
            return *addr;
        }

        if (pc) {
            return proc.pc;
        }

        throw std::runtime_error("Unsupported operand.");
    }

    uint32_t Value(ESize sz, const std::vector<uint8_t>& ram) const {
        if (addr) {
            uint32_t value = ram[*addr];
            if (sz == ESize::W || sz == ESize::L) {
                value |= ram[*addr + 1] << 8;
            }
            if (sz == ESize::L) {
                value |= ram[*addr + 2] << 16 | ram[*addr + 3] << 24;
            }
            return value;
        }

        if (imm) {
            return *imm;
        }

        throw std::runtime_error("Unsupported operand.");
    }

    void Write(ESize sz, uint32_t value, TProcessor& proc, std::vector<uint8_t>& ram) const {
        if (direct) {
            proc.reg[*direct] = value;
        }

        else {
            throw std::runtime_error("Unsupported operand.");
        }
    }
};
