#pragma once

enum struct ESize {
    Byte,
    Word,
    Long
};

ESize GetSize(u16 opcode) {
    switch (opcode & 0xc0) {
        case 0x0:
            return ESize::Byte;
        case 0x40:
            return ESize::Word;
        case 0x80:
            return ESize::Long;
        default:
            throw std::runtime_error("Wrong size code.");
    }
}

enum struct EAddrMode {
    DataReg,
    AddrReg,
    Addr,
    AddrInc,
    AddrDec,
    AddrDisp,
    AddrIdx,
    PCDisp,
    PCIdx,
    AbsShort,
    AbsLong,
    Imm
};

EAddrMode GetAddrMode(u16 opcode) {
    switch ((opcode & 0x0038) >> 3) {
        case 0x0:
            return EAddrMode::DataReg;
        default:
            throw std::runtime_error("Wrong addressing mode.");
    }
}
