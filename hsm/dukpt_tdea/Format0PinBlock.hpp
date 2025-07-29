#pragma once
#include <vector>
#include <string>
#include <cstdint>

class Format0PinBlock {
public:
    static std::vector<uint8_t> generate(const std::string& pin, const std::string& pan);
};
