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
        TString line;
        while (std::getline(inputFile, line)) {
            if (
                line.find(' ') != TString::npos &&
                line.find(':') != TString::npos &&
                line.find('\t') != TString::npos
            ) {
                LoadLine(line);
            }
        }
    }

    void LoadLine(const TString& line) {
        auto tokens = line.split('\t', 1);

        auto addr = tokens[0].strip();
        addr.pop_back();
        auto numeric_addr = hex2int(addr);

        auto data = tokens[1].strip();
        TString instr;
        if (data.find('\t') != TString::npos) {
            tokens = data.split('\t', 1);
            data = tokens[0].strip();
            instr = tokens[1].strip();
        }

        for (const auto& word : data.split(' ')) {
            std::optional<TInstruction> instruction;
            if (!instr.empty()) {
                TString cmd = instr;
                std::vector<std::unique_ptr<TArg>> args;
                if (instr.find(' ') != TString::npos) {
                    auto instr_tokens = instr.split(' ', 1);
                    cmd = instr_tokens[0];
                    if (instr_tokens[1].find(',') != TString::npos) {
                        auto raw_args = instr_tokens[1].split(',');
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
                    static_cast<uint32_t>(2 * data.split(' ').size()),
                    cmd,
                    std::move(args)
                };
                instr = TString{};
            }

            TAddress address = {
                {
                    static_cast<uint8_t>(hex2int(TString{word.substr(0, 2)})), 
                    static_cast<uint8_t>(hex2int(TString{word.substr(2, 2)}))
                }, 
                std::move(instruction)
            };
            memory[numeric_addr] = std::move(address);
            numeric_addr += 2;
        }
    }
};
