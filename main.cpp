#include <iostream>

#include "TMemory.h"

int main() {
    try {
        TMemory memory;
        memory.LoadROM("test.asm");
        memory.InitRAM(1 << 24);

        TProcessor processor{0x200};

        std::cout << "Starting CPU..." << std::endl << std::endl;

        for (;;) {
            if (processor.pc >= memory.memory.size()) {
                throw std::runtime_error("Invalid instruction.");
            }

            const auto& instruction = memory.memory[processor.pc].instruction;
            if (!instruction.has_value()) {
                throw std::runtime_error("Invalid instruction.");
            }

            processor.run(*instruction, memory.memory);
        }
    } catch (std::runtime_error& e) {
        std::cout << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
