#pragma once
#include <vector>
#include <cstdint>

class DukptHsm {
public:
    static std::vector<uint8_t> aesDecryptECB(
        const std::vector<uint8_t>& key,
        const std::vector<uint8_t>& data);
};
