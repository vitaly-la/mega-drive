#pragma once

#include <sstream>
#include <string>
#include <vector>

struct TString : std::string {
    std::vector<TString> split(char delim, ssize_t maxsplit = -1) const {
        std::vector<TString> tokens;
        std::stringstream ss{*this};
        TString token;
        size_t cnt{};
        while (std::getline(ss, token, delim)) {
            if (maxsplit != -1 && cnt >= maxsplit) {
                if (ss.tellg() != TString::npos) {
                    token += delim + ss.str().substr(ss.tellg());
                }
                tokens.push_back(TString{token});
                break;
            }
            tokens.push_back(token);
            ++cnt;
        }
        return tokens;
    }

    TString strip() const {
        size_t start = this->find_first_not_of(" \t\n\r\f\v");
        size_t end = this->find_last_not_of(" \t\n\r\f\v");
        if (start == TString::npos || end == TString::npos) {
            return TString{};
        }
        return TString{this->substr(start, end - start + 1)};
    }
};
