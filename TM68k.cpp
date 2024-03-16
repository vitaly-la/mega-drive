#include "common.h"

#include "TMemory.h"
#include "TM68k.h"
#include "opcodes.h"

void TM68k::ProcessInstruction() {
    std::cout << std::hex << PC << ": " << Instructions.at(PC) << "\n";

    u16 opcode = Read<u16>();

    Process(std::array{0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 1}, opcode, []() { // NOP
        std::cout << "nop\n";
    });

    switch (opcode) {
    case 0x003c: // ORI to CCR
        SR |= Read<u8>();
        return;
    case 0x007c: // ORI to SR
        SR |= Read<u16>();
        return;
    case 0x023c: // ANDI to CCR
        SR &= Read<u8>();
        return;
    case 0x027c: // ANDI to SR
        SR &= Read<u16>();
        return;
    case 0x0a3c: // EORI to CCR
        SR ^= Read<u8>();
        return;
    case 0x0a7c: // EORI to SR
        SR ^= Read<u16>();
        return;
    case 0x4e72: // STOP
        SR = Read<u16>();
        Status = EStatus::Stopped;
        return;
    }

    if ((opcode & 0xffc0) == 0x4ec0) { // JMP
        PC = Read<u32>();
        // Other addressing modes
        return;
    }

    throw std::runtime_error("Unsupported instruction.");
}
