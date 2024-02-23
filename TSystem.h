#pragma once

#include <optional>
#include <string>

struct TProcessor;

/*================ ARGUMENT TYPES ================*/

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

    // Methods
};

/*================ SYSTEM COMPONENTS ================*/

struct TInstruction {
    uint8_t size;
    std::string cmd;
    std::vector<TArg> args;
};

void process_instruction(const TInstruction&, std::vector<uint8_t>&, TProcessor&);

struct TProcessor {
    uint32_t pc;
    uint16_t sr;
    uint32_t reg[16];

    explicit TProcessor(uint32_t pc) :
        pc{pc},
        sr{},
        reg{}
    {}

    void run(const TInstruction& instruction, std::vector<uint8_t>& ram) {
        process_instruction(instruction, ram, *this);
    }
};
