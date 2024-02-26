#pragma once

struct TM68k {
    TMemory Memory;
    std::map<u32, std::string> Instructions;
    u32 Reg[16];
    u32 SSP;
    u32 PC;
    u16 SR;

    TM68k(TMemory memory, const std::string& asmFile) : 
        Memory{std::move(memory)},
        Reg{},
        SR{}
    {
        ReadInstructions(asmFile);

        SSP = Memory.Read<u32>(0x0);
        PC = Memory.Read<u32>(0x4);
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
                    u32 addr = Hex2int(tokens[0]);
                    Instructions[addr] = tokens[2];
                }
            }
        }
    }
};
