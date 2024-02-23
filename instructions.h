#pragma once

#include <bitset>
#include <chrono>
#include <thread>

#include "TSystem.h"

constexpr uint16_t C_FLAG{ 1 << 0 };
constexpr uint16_t V_FLAG{ 1 << 1 };
constexpr uint16_t Z_FLAG{ 1 << 2 };
constexpr uint16_t N_FLAG{ 1 << 3 };
constexpr uint16_t X_FLAG{ 1 << 4 };

TArg parse_arg(const std::string& arg) {
    if (arg.rfind("#", 0) == 0) {
        return TArg{
            EArgType::Immediate, 
            static_cast<uint32_t>(std::stoi(arg.substr(1))), 
            {}
        };
    }

    if (arg.rfind("0x", 0) == 0) {
        return TArg{EArgType::Address, {}, hex2int(arg)};
    }

    return TArg{EArgType::Unknown, {}, {}};
}

void process_instruction(const TInstruction& instruction, std::vector<uint8_t>& ram, TProcessor& proc) {
    std::cout << std::hex << proc.pc << ": " << instruction.cmd << " ";
    for (size_t i = 0; i < instruction.args.size(); ++i) {
        auto arg = instruction.args[i];
        if (arg.type == EArgType::Immediate) {
            std::cout << "#" << std::dec << arg.value;
        } else {
            std::cout << "0x" << std::hex << arg.addr;
        }
        if (i < instruction.args.size() - 1) {
            std::cout << ",";
        }
    }
    std::cout << std::endl << "SR: " << std::bitset<16>(proc.sr) << std::endl << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(1));

    if (instruction.cmd == "nop") {
        proc.pc += instruction.size;
    }

    else if (instruction.cmd == "jmp") {
        proc.pc = instruction.args[0].addr;
    }

    else if (instruction.cmd == "tstl") {
        auto addr = instruction.args[0].addr;

        uint32_t value = ram[addr + 3] << 24 |
                         ram[addr + 2] << 16 |
                         ram[addr + 1] << 8 |
                         ram[addr];

        if (value == 0) {
            proc.sr |= Z_FLAG;
        } else {
            proc.sr &= ~Z_FLAG;
        }

        proc.sr &= ~C_FLAG & ~V_FLAG & ~N_FLAG;
        
        proc.pc += instruction.size;
    }

    else if (instruction.cmd == "bnes") {
        if (proc.sr & Z_FLAG) {
            proc.pc += instruction.size;
        } else {
            proc.pc = instruction.args[0].addr;
        }
    }

    else {
        throw std::runtime_error("Unsupported instruction.");
    }
}
