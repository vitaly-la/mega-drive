#pragma once

#include <optional>

#include "common.h"

struct TMemory;
struct TProcessor;

/*================ ARGUMENT TYPES ================*/

struct TArg {
    virtual std::string Str() const = 0;
    virtual ~TArg() {}
};

struct TAddressArg : TArg {
    uint32_t addr;
    explicit TAddressArg(uint32_t addr) : TArg{}, addr{addr} {}
    std::string Str() const override {
        std::stringstream ss;
        ss << std::hex << addr;
        return ss.str();
    }
};

struct TLiteralArg : TArg {
    uint32_t value;
    explicit TLiteralArg(uint32_t value) : TArg{}, value{value} {}
    std::string Str() const override {
        std::stringstream ss;
        ss << std::hex << value;
        return ss.str();
    }
};

/*================ SYSTEM COMPONENTS ================*/

struct TInstruction {
    uint32_t size;
    std::string cmd;
    std::vector<std::unique_ptr<TArg>> args;
};

struct TAddress {
    uint8_t data[2];
    std::optional<TInstruction> instruction;
};

void process_instruction(const TInstruction&, std::map<uint32_t, TAddress>&, TProcessor&);

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

    void run(const TInstruction& instruction, std::map<uint32_t, TAddress>& ram) {
        process_instruction(instruction, ram, *this);
    }
};
