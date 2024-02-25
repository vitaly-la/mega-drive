#pragma once

#include <map>

#include "common.h"

struct TM68k {
    std::unique_ptr<TMemory> Memory;
    std::map<uint32_t, std::string> Instructions;
    uint32_t PC;

    TM68k(std::unique_ptr<TMemory> memory, const std::string& asmFile) : Memory{std::move(memory)} {
        ReadInstructions(asmFile);

        PC = Memory->Read32(0x4);
    }

    void Run() {
        std::cout << std::hex << PC << ": " << Instructions[PC] << std::endl;
    }

    void ReadInstructions(const std::string& asmFile) {
        std::ifstream input{asmFile, std::ios::binary};
        std::string line;
        while (std::getline(input, line)) {
            if (
                line.find(':') != std::string::npos &&
                line.find('\t') != std::string::npos
            ) {
                auto tokens = Split(line, '\t');
                if (tokens.size() >= 3) {
                    uint32_t addr = Hex2int(tokens[0]);
                    Instructions[addr] = tokens[2];
                }
            }
        }
    }
};
