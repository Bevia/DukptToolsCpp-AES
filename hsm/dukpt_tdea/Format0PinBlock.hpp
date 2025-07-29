#pragma once
#include <vector>
#include <string>
#include <cstdint>

class Format0PinBlock {
public:
    static std::vector<uint8_t> generate(const std::string& pin, const std::string& pan);
    static std::string extract(const std::vector<uint8_t>& decryptedBlock, const std::string& pan); // 👈 new method
};
