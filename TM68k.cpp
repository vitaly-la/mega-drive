#include "common.h"

#include "TMemory.h"
#include "TM68k.h"

void TM68k::ProcessInstruction() {
    std::cout << std::hex << PC << ": " << Instructions[PC] << std::endl;

    if (Memory.Read<u16>(PC) == 0x4e71) { // NOP
        PC += 2;
    }

    else if (Memory.Read<u16>(PC) == 0x4e72) { // STOP
        SR = Memory.Read<u16>(PC + 2);
        PC += 4;
        Status = EStatus::Stopped;
    }

    else {
        throw std::runtime_error("Unsupported instruction.");
    }
}
