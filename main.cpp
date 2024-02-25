#include <iostream>
#include <memory>

#include "TMemory.h"
#include "TROM.h"
#include "TRAM.h"
#include "TM68k.h"

int main() {
    try {
        auto memory = std::make_unique<TMemory>();

        memory->Connect(std::make_unique<TROM>("rom.bin"));
        memory->Connect(std::make_unique<TRAM>(0xFF0000, 0x10000));

        TM68k m68k{std::move(memory), "rom.asm"};

        m68k.Run();
    } catch (std::runtime_error& e) {
        std::cout << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
