#pragma once

#include <sstream>

uint32_t hex2int(const std::string& hex) {
    uint32_t x;
    std::stringstream ss;
    ss << std::hex << hex;
    ss >> x;
    return x;
}

std::vector<std::string> split(const std::string& src, char delim, ssize_t maxsplit = -1) {
    std::vector<std::string> tokens;
    std::stringstream ss{src};
    std::string token;
    size_t cnt{};
    while (std::getline(ss, token, delim)) {
        if (maxsplit != -1 && cnt >= maxsplit) {
            if (ss.tellg() != std::string::npos) {
                token += delim + ss.str().substr(ss.tellg());
            }
            tokens.push_back(token);
            break;
        }
        tokens.push_back(token);
        ++cnt;
    }
    return tokens;
}

std::string strip(const std::string& src) {
    size_t start = src.find_first_not_of(" \t\n\r\f\v");
    size_t end = src.find_last_not_of(" \t\n\r\f\v");
    if (start == std::string::npos || end == std::string::npos) {
        return "";
    }
    return src.substr(start, end - start + 1);
}
