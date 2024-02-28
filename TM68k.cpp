#include "common.h"

#include "TMemory.h"
#include "TM68k.h"

void TM68k::ProcessInstruction() {
    std::cout << std::hex << PC << ": " << Instructions[PC] << std::endl;

    u16 opcode = Memory.Read<u16>(PC);
    PC += 2;

    if (opcode == 0x003c) { // ORI to CCR
        SR |= Memory.Read<u8>(PC);
        PC += 1;
    }

    else if (opcode == 0x007c) { // ORI to SR
        SR |= Memory.Read<u16>(PC);
        PC += 2;
    }

    else if (opcode == 0x023c) { // ANDI to CCR
        SR &= Memory.Read<u8>(PC);
        PC += 1;
    }

    else if (opcode == 0x027c) { // ANDI to SR
        SR &= Memory.Read<u16>(PC);
        PC += 2;
    }

    else if (opcode == 0x4e71) {} // NOP

    else if (opcode == 0x4e72) { // STOP
        SR = Memory.Read<u16>(PC);
        PC += 2;
        Status = EStatus::Stopped;
    }

    else if ((opcode & 0xffc0) == 0x4ec0) { // JMP
        PC = Memory.Read<u32>(PC);
    }

    else {
        throw std::runtime_error("Unsupported instruction.");
    }
}
