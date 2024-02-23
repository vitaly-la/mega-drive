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
    TArg parsed{};

    if (arg.rfind("%", 0) == 0) {
        if (arg.find("@") == std::string::npos) {
            if (arg[1] == 'd' && std::isdigit(arg[2])) {
                parsed.direct = std::stoi(arg.substr(2, 1));
            } else if (arg[1] == 'a' && std::isdigit(arg[2])) {
                parsed.direct = 8 + std::stoi(arg.substr(2, 1));
            }
        } else {
            if (arg.substr(1, 2) == "pc") {
                parsed.pc = true;
            }
        }
    }

    else if (arg.rfind("0x", 0) == 0) {
        parsed.addr = hex2int(arg);
    }

    else if (arg.rfind("#", 0) == 0) {
        parsed.imm = std::stoi(arg.substr(1));
    }

    return parsed;
}

void process_instruction(const TInstruction& instr, TProcessor& proc, std::vector<uint8_t>& ram) {
    std::cout << std::hex << proc.pc << ": " << instr.text << std::endl;
    for (size_t i = 0; i < 16; ++i) {
        if (i == 8) {
            std::cout << std::endl;
        }
        if (i < 8) {
            std::cout << "D" << i << ": " << std::dec << proc.reg[i] << "\t";
        } else {
            std::cout << "A" << i - 8 << ": 0x" << std::hex << proc.reg[i] << "\t";
        }
    }
    std::cout << std::endl << "SR: " << std::bitset<16>(proc.sr) << std::endl << std::endl;

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    if (instr.cmd == "nop") {
        proc.pc += instr.size;
    }

    else if (instr.cmd == "jmp") {
        proc.pc = instr.args[0].Addr(proc);
    }

    else if (instr.cmd.rfind("tst", 0) == 0) {
        auto sz{ESize::B};
        if (instr.cmd[3] == 'w') sz = ESize::W;
        if (instr.cmd[3] == 'l') sz = ESize::L;

        auto value = instr.args[0].Value(sz, ram);

        if (value == 0) {
            proc.sr |= Z_FLAG;
        } else {
            proc.sr &= ~Z_FLAG;
        }

        proc.sr &= ~C_FLAG & ~V_FLAG & ~N_FLAG;
        
        proc.pc += instr.size;
    }

    else if (instr.cmd.rfind("bne", 0) == 0) {
        if (proc.sr & Z_FLAG) {
            proc.pc += instr.size;
        } else {
            proc.pc = instr.args[0].Addr(proc);
        }
    }

    else if (instr.cmd == "lea") {
        instr.args[1].Write(ESize::L, instr.args[0].Addr(proc), proc, ram);
        proc.pc += instr.size;
    }

    else {
        throw std::runtime_error("Unsupported instruction.");
    }
}
