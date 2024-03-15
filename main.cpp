#include "common.h"

#include "TMemory.h"
#include "TROM.h"
#include "TRAM.h"
#include "TM68k.h"

int main() {
    try {
        TMemory memory;

        memory.Connect(TROM{"rom.bin"});
        memory.Connect(TRAM{0xFF0000, 0x10000});

        TM68k m68k{std::move(memory), "rom.asm"};

        std::cout << "Starting M68k...\n";
        m68k.Run();
        std::cout << "M68k stopped.\n";
    } catch (std::runtime_error& e) {
        std::cout << e.what() << "\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
