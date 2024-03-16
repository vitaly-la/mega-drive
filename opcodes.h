#pragma once

template<size_t Size>
void Process(const std::array<int, Size>& mask, u16 opcode, std::function<void()> process) {
    process();
}
