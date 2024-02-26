#pragma once

struct TM68k {
    std::unique_ptr<TMemory> Memory;
    std::map<uint32_t, std::string> Instructions;
    uint32_t Reg[16];
    uint32_t SSP;
    uint32_t PC;
    uint16_t SR;

    TM68k(std::unique_ptr<TMemory> memory, const std::string& asmFile) : 
        Memory{std::move(memory)},
        Reg{},
        SR{}
    {
        ReadInstructions(asmFile);

        SSP = Memory->Read32(0x0);
        PC = Memory->Read32(0x4);
    }

    void Run() {
        for (;;) {
            ProcessInstruction();
        }
    }

    void ProcessInstruction();

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
