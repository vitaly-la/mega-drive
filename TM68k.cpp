#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <vector>

#include "common.h"

#include "TMemory.h"
#include "TM68k.h"

void TM68k::ProcessInstruction() {
    std::cout << std::hex << PC << ": " << Instructions[PC] << std::endl;

    if (Memory->Read32(PC) == 0x4e71) { // NOP
        PC += 2;
    }

    else {
        throw std::runtime_error("Unsupported instruction.");
    }
}