#pragma once

#include <bitset>
#include <chrono>
#include <thread>

#include "TSystem.h"

constexpr uint32_t C_FLAG = 1 << 0;
constexpr uint32_t V_FLAG = 1 << 1;
constexpr uint32_t Z_FLAG = 1 << 2;
constexpr uint32_t N_FLAG = 1 << 3;
constexpr uint32_t X_FLAG = 1 << 4;

std::unique_ptr<TArg> parse_arg(const std::string& arg) {
    TArg* pArg{nullptr};

    if (arg.rfind("0x", 0) == 0) {
        pArg = new TAddressArg{hex2int(arg)};
    }

    else if (arg.rfind("#", 0) == 0) {
        pArg = new TLiteralArg{hex2int(arg.substr(1))};
    }

    return std::unique_ptr<TArg>(pArg);
}

void process_instruction(const TInstruction& instruction, std::map<uint32_t, TAddress>& ram, TProcessor& proc) {
    //std::this_thread::sleep_for(std::chrono::seconds(1));
    
    std::cout
        << "PC: " << std::hex << proc.pc
        << ", instruction: " << instruction.cmd << " ";
    for (size_t i = 0; i < instruction.args.size(); ++i) {
        std::cout << instruction.args[i]->Str();
        if (i < instruction.args.size() - 1) {
            std::cout << ",";
        } else {
            std::cout << std::endl;
        }
    }
    std::cout
        << "CCR: " << std::bitset<16>(proc.ccr)
        << std::endl << std::endl;

    if (instruction.cmd == "nop" || instruction.cmd == ".short") {
        proc.pc += instruction.size;
    }

    else if (instruction.cmd == "jmp") {
        proc.pc = static_cast<TAddressArg*>(instruction.args[0].get())->addr;
    }

    else if (instruction.cmd == "tstl") {
        auto addr = static_cast<TAddressArg*>(instruction.args[0].get())->addr;

        uint32_t value =
            ram.at(addr + 2).data[1] << 24 |
            ram.at(addr + 2).data[0] << 16 |
            ram.at(addr).data[1] << 8 |
            ram.at(addr).data[0];

        if (value == 0) {
            proc.ccr |= Z_FLAG;
        } else {
            proc.ccr &= ~Z_FLAG;
        }

        proc.ccr &= ~N_FLAG;
        proc.ccr &= ~V_FLAG;
        proc.ccr &= ~C_FLAG;
        
        proc.pc += instruction.size;
    }

    else {
        throw std::runtime_error("Unsupported instruction.");
    }
}
