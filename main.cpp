#include "TMemory.h"

int main() {
    try {
        TMemory memory;
        memory.InitRAM(1 << 24);
        memory.LoadROM("test.asm");

        TProcessor processor{0x200};

        std::cout << "Starting CPU..." << std::endl << std::endl;

        for (;;) {
            const auto& instruction = memory.instructions.at(processor.pc);
            processor.run(instruction, memory.data);
        }
    } catch (std::out_of_range&) {
        std::cout << "Invalid instruction." << std::endl;
        return EXIT_FAILURE;
    } catch (std::runtime_error& e) {
        std::cout << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
