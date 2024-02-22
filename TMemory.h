#pragma once

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>

#include "instructions.h"

struct TMemory {
    std::map<uint32_t, TAddress> memory;

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
        auto numeric_addr = hex2int(addr);

        auto data = strip(tokens[1]);
        std::string instr;
        if (data.find('\t') != std::string::npos) {
            tokens = split(data, '\t', 1);
            data = strip(tokens[0]);
            instr = strip(tokens[1]);
        }

        for (const auto& word : split(data, ' ')) {
            std::optional<TInstruction> instruction;
            if (!instr.empty()) {
                std::string cmd = instr;
                std::vector<std::unique_ptr<TArg>> args;
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
                    static_cast<uint32_t>(2 * split(data, ' ').size()),
                    cmd,
                    std::move(args)
                };
                instr = "";
            }

            TAddress address = {
                {
                    static_cast<uint8_t>(hex2int(word.substr(0, 2))), 
                    static_cast<uint8_t>(hex2int(word.substr(2, 2)))
                }, 
                std::move(instruction)
            };
            memory[numeric_addr] = std::move(address);
            numeric_addr += 2;
        }
    }

    void InitRAM(uint32_t size) {
        auto addr = memory.rbegin()->first + 2;
        for (uint32_t i = 0; i < size; i += 2) {
            memory[addr] = TAddress{};
            addr += 2;
        }
    }
};
