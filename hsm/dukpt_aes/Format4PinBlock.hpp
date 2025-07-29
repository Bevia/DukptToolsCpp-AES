#pragma once
#include <vector>
#include <string>

class Format4PinBlock {
public:
    static std::vector<uint8_t> generate(const std::string &pin, const std::string &pan);
};
