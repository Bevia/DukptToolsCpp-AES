#pragma once

#include <vector>
#include <string>
#include "AesMode.hpp"

class AesHsmSimulator {
public:
    explicit AesHsmSimulator(const std::vector<uint8_t>& bdk);

    // ECB decryption — expects PAN
    std::string decryptPin(AesMode mode,
                           const std::vector<uint8_t>& encrypted,
                           const std::vector<uint8_t>& ksn,
                           const std::string& pan);

    // CBC decryption — expects IV
    std::string decryptPin(AesMode mode,
                           const std::vector<uint8_t>& encrypted,
                           const std::vector<uint8_t>& ksn,
                           const std::vector<uint8_t>& iv);

private:
    std::vector<uint8_t> aesDecryptEcb(const std::vector<uint8_t>& key,
                                       const std::vector<uint8_t>& encrypted);

    std::vector<uint8_t> aesDecryptCbc(const std::vector<uint8_t>& key,
                                       const std::vector<uint8_t>& iv,
                                       const std::vector<uint8_t>& encrypted);

    std::string extractPinFromBlockEcb(const std::vector<uint8_t>& pinBlock,
                                       const std::string& pan);

    std::string extractPinFromBlockCbc(const std::vector<uint8_t>& pinBlock);

    std::vector<uint8_t> bdk_;
};
