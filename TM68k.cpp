#include "common.h"

#include "TMemory.h"
#include "TM68k.h"

void TM68k::ProcessInstruction() {
    std::cout << std::hex << PC << ": " << Instructions.at(PC) << std::endl;

    u16 opcode = Read<u16>();

    if (opcode == 0x003c) { // ORI to CCR
        SR |= Read<u8>();
    }

    else if (opcode == 0x007c) { // ORI to SR
        SR |= Read<u16>();
    }

    else if (opcode == 0x023c) { // ANDI to CCR
        SR &= Read<u8>();
    }

    else if (opcode == 0x027c) { // ANDI to SR
        SR &= Read<u16>();
    }

    else if (opcode == 0x4e71) {} // NOP

    else if (opcode == 0x4e72) { // STOP
        SR = Read<u16>();
        Status = EStatus::Stopped;
    }

    else if ((opcode & 0xffc0) == 0x4ec0) { // JMP
        PC = Read<u32>();
        // Other addressing modes
    }

    else {
        throw std::runtime_error("Unsupported instruction.");
    }
}
