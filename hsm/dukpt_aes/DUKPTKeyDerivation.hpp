#pragma once
#include <vector>
#include <cstdint>

class DUKPTKeyDerivation {
public:
    DUKPTKeyDerivation(const std::vector<uint8_t>& bdk);
    std::vector<uint8_t> deriveKey(const std::vector<uint8_t>& ksn);
    std::vector<uint8_t> generateIPEK(const std::vector<uint8_t>& ksn);
    std::vector<uint8_t> aesEncryptECB(const std::vector<uint8_t>& key, const std::vector<uint8_t>& data);
    std::vector<uint8_t> aesDecryptECB(const std::vector<uint8_t>& key, const std::vector<uint8_t>& data);

private:
    std::vector<uint8_t> baseDerivationKey;
    std::vector<uint8_t> nonReversibleKeyDerivation(const std::vector<uint8_t>& ipek, const std::vector<uint8_t>& ksn);
};
