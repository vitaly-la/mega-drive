#pragma once

inline std::vector<std::string> Split(const std::string& src, char delim, ssize_t maxsplit = -1) {
    std::vector<std::string> tokens;
    std::stringstream ss{src};
    std::string token;
    ssize_t cnt{};
    while (std::getline(ss, token, delim)) {
        if (maxsplit != -1 && cnt >= maxsplit) {
            if (ss.tellg() != -1) {
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

inline uint32_t Hex2int(const std::string& hex) {
    uint32_t x;
    std::stringstream ss;
    ss << std::hex << hex;
    ss >> x;
    return x;
}
