#pragma once

#include <string>

struct TProcessor;

/*================ ARGUMENT TYPES ================*/

enum struct EArgType {
    Immediate,
    Address,
    Unknown
};

struct TArg {
    EArgType type;
    uint32_t value;
    uint32_t addr;
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
    uint32_t d[8];
    uint32_t a[8];

    explicit TProcessor(uint32_t pc) :
        pc{pc},
        sr{},
        d{},
        a{}
    {}

    void run(const TInstruction& instruction, std::vector<uint8_t>& ram) {
        process_instruction(instruction, ram, *this);
    }
};
