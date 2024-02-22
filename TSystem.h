#pragma once

#include <memory>
#include <string>

struct TProcessor;

/*================ ARGUMENT TYPES ================*/

struct TArg {
    virtual std::string Str() const = 0;
    virtual ~TArg() {}
};

struct TImmediateArg : TArg {
    uint32_t imm;
    explicit TImmediateArg(uint32_t imm) : TArg{}, imm{imm} {}
    std::string Str() const override {
        std::stringstream ss;
        ss << "#" << imm;
        return ss.str();
    }
};

struct TAddressArg : TArg {
    uint32_t addr;
    explicit TAddressArg(uint32_t addr) : TArg{}, addr{addr} {}
    std::string Str() const override {
        std::stringstream ss;
        ss << "0x" << std::hex << addr;
        return ss.str();
    }
};

/*================ SYSTEM COMPONENTS ================*/

struct TInstruction {
    uint8_t size;
    std::string cmd;
    std::vector<std::unique_ptr<TArg>> args;
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
