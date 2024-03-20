#include "common.h"

#include "TMemory.h"
#include "TM68k.h"
#include "opcodes.h"

void TM68k::ProcessInstruction() {
    std::cout << std::hex << PC << ": " << Instructions.at(PC) << "\n";

    u16 opcode = Read<u16>();

    if (Process(std::array{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0}, opcode, // ORI to CCR
        [this](const TInstruction&) {
            CCR |= Read<u8>();
        }
    )) return;

    if (Process(std::array{0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0}, opcode, // ORI to SR
        [this](const TInstruction&) {
            SR |= Read<u16>();
        }
    )) return;

    // TODO: set CCR, check endianness
    if (Process(std::array{0, 0, 0, 0, 0, 0, 0, 0, S, M, Xn}, opcode, // ORI
        [this](const TInstruction& instruction) {
            switch (instruction.size) {
                case ESize::Byte: {
                    u8 data = Read<u8>();
                    switch (instruction.mode) {
                        case EMode::Dn:
                            D[instruction.reg] |= data;
                            break;
                    }
                    break;
                }
            }
        }
    )) return;

    if (Process(std::array{0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 1}, opcode, // NOP
        [](const TInstruction&) {}
    )) return;

    if (Process(std::array{0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0}, opcode, // STOP
        [this](const TInstruction&) {
            SR = Read<u16>();
            Status = EStatus::Stopped;
        }
    )) return;

    throw std::runtime_error("Unsupported instruction.");
}
