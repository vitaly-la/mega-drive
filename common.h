#pragma once

#include <sstream>

#include "TString.h"

uint32_t hex2int(const TString& hex) {
    uint32_t x;
    std::stringstream ss;
    ss << std::hex << hex;
    ss >> x;
    return x;
}
