#pragma once

enum struct EStatus {
    Init,
    Running,
    Stopped
};

struct TM68k {
    EStatus Status;
    TMemory Memory;
    std::map<u32, std::string> Instructions;
    u32 Reg[16];
    u32 SSP;
    u32 PC;
    u16 SR;

    TM68k(TMemory memory, const std::string& asmFile) : 
        Status{EStatus::Init},
        Memory{std::move(memory)},
        Reg{},
        SR{}
    {
        ReadInstructions(asmFile);

        SSP = Memory.Read<u32>(0x0);
        PC = Memory.Read<u32>(0x4);
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
                    u32 addr = Hex2int(tokens[0]);
                    Instructions[addr] = tokens[2];
                }
            }
        }
    }

    void Run() {
        Status = EStatus::Running;
        while (Status == EStatus::Running) {
            ProcessInstruction();
        }
    }

    void ProcessInstruction();

    template<class T>
    T Read() {
        auto value = Memory.Read<T>(PC);
        PC += sizeof(T) / sizeof(u8);
        return value;
    }
};
