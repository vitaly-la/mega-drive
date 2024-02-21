#pragma once

#include <optional>

#include "common.h"

struct TMemory;
struct TProcessor;

struct TArg {};

struct TAddressArg : TArg {
    uint32_t addr;
    explicit TAddressArg(uint32_t addr) : TArg{}, addr{addr} {}
};

struct TInstruction {
    uint32_t size;
    TString cmd;
    std::vector<std::unique_ptr<TArg>> args;
};

struct TAddress {
    uint8_t data[2];
    std::optional<TInstruction> instruction;
};

void process_instruction(const TInstruction&, TMemory&, TProcessor&);

struct TProcessor {
    uint32_t pc;
    uint32_t ccr;
    uint32_t data[8];
    uint32_t addr[7];
    uint32_t usp;
    uint32_t ssp;

    explicit TProcessor(uint32_t pc) :
        pc{pc},
        ccr{},
        data{},
        addr{},
        usp{},
        ssp{}
    {}

    void run(const TInstruction& instruction, TMemory& memory) {
        process_instruction(instruction, memory, *this);
    }
};
