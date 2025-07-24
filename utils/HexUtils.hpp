#pragma once

#include <vector>
#include <string>

class HexUtils {
public:
    static std::vector<uint8_t> hexToBytes(const std::string& hex);
    static std::string bytesToHex(const std::vector<uint8_t>& bytes);
};
