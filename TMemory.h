#pragma once

#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <map>

#include "common.h"
#include "instructions.h"

struct TMemory {
    std::vector<uint8_t> data;
    std::map<uint32_t, TInstruction> instructions;

    void InitRAM(uint32_t size) {
        data = std::vector<uint8_t>(size, 0);
    }

    void LoadROM(const char path[]) {
        std::cout << "Loading " << path << "..." << std::endl;
        std::ifstream inputFile{path};
        std::string line;
        while (std::getline(inputFile, line)) {
            if (
                line.find(' ') != std::string::npos &&
                line.find(':') != std::string::npos &&
                line.find('\t') != std::string::npos
            ) {
                LoadLine(line);
            }
        }
    }

    void LoadLine(const std::string& line) {
        auto tokens = split(line, '\t', 1);

        auto addr = strip(tokens[0]);
        addr.pop_back();
        uint32_t numeric_addr = hex2int(addr);

        auto bytes = strip(tokens[1]);
        std::string instr;
        if (bytes.find('\t') != std::string::npos) {
            tokens = split(bytes, '\t', 1);
            bytes = strip(tokens[0]);
            instr = strip(tokens[1]);
        }

        for (const auto& word : split(bytes, ' ')) {
            TInstruction instruction{};
            if (!instr.empty()) {
                std::string cmd{instr};
                std::vector<TArg> args;
                if (instr.find(' ') != std::string::npos) {
                    auto instr_tokens = split(instr, ' ', 1);
                    cmd = instr_tokens[0];
                    if (instr_tokens[1].find(',') != std::string::npos) {
                        auto raw_args = split(instr_tokens[1], ',');
                        std::transform(
                            std::cbegin(raw_args),
                            std::cend(raw_args),
                            std::back_inserter(args),
                            parse_arg
                        );
                    } else {
                        args.push_back(parse_arg(instr_tokens[1]));
                    }
                }

                instruction = {
                    static_cast<uint8_t>(2 * split(bytes, ' ').size()),
                    instr,
                    cmd,
                    std::move(args)
                };
                instr = "";
            }

            data[numeric_addr] = static_cast<uint8_t>(hex2int(word.substr(0, 2)));
            data[numeric_addr + 1] = static_cast<uint8_t>(hex2int(word.substr(2, 2)));
            instructions[numeric_addr] = std::move(instruction);
            numeric_addr += 2;
        }
    }

};
