#pragma once
#include <vector>
#include <cstdint>

class DukptHsm {
public:
    // Triple DES ECB decryption
    std::vector<uint8_t> tdeaDecryptECB(const std::vector<uint8_t>& key, const std::vector<uint8_t>& data);
};
