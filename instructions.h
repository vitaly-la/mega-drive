#pragma once

#include <bitset>
#include <chrono>
#include <thread>

#include "TSystem.h"

constexpr uint32_t BIT_CARRY    = 1 << 0;
constexpr uint32_t BIT_OVERFLOW = 1 << 1;
constexpr uint32_t BIT_ZERO     = 1 << 2;
constexpr uint32_t BIT_NEGATIVE = 1 << 3;

std::unique_ptr<TArg> parse_arg(const TString& arg) {
    return std::unique_ptr<TArg>(new TAddressArg{hex2int(arg)});
}

void process_instruction(const TInstruction& instruction, TMemory& memory, TProcessor& processor) {
    //std::this_thread::sleep_for(std::chrono::seconds(1));
    
    std::cout
        << "PC: " << std::hex << processor.pc
        << ", instruction: " << instruction.cmd << " ";
    for (size_t i = 0; i < instruction.args.size(); ++i) {
        std::cout << static_cast<TAddressArg*>(instruction.args[i].get())->addr;
        if (i < instruction.args.size() - 1) {
            std::cout << ",";
        } else {
            std::cout << std::endl;
        }
    }
    std::cout
        << "CCR: " << std::bitset<16>(processor.ccr)
        << std::endl << std::endl;

    if (instruction.cmd == "nop" || instruction.cmd == ".short") {
        processor.pc += instruction.size;
    }

    else if (instruction.cmd == "jmp") {
        processor.pc = static_cast<TAddressArg*>(instruction.args[0].get())->addr;
    }

    else if (instruction.cmd == "tstl") {
        //
        processor.pc += instruction.size;
    }

    else {
        throw std::runtime_error("Unsupported instruction.");
    }
}
