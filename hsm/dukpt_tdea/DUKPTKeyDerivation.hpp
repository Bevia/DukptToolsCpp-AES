#pragma once
#include <vector>
#include <cstdint>

class DUKPTKeyDerivation {
public:
    // Constructor with Base Derivation Key
    DUKPTKeyDerivation(const std::vector<uint8_t>& bdk);

    // Derive session key from KSN
    std::vector<uint8_t> deriveKey(const std::vector<uint8_t>& ksn);

    // Generate Initial PIN Encryption Key from KSN
    std::vector<uint8_t> generateIPEK(const std::vector<uint8_t>& ksn);

    // Triple DES ECB encryption
    std::vector<uint8_t> tdeaEncryptECB(const std::vector<uint8_t>& key, const std::vector<uint8_t>& data);

private:
    // Base derivation key (BDK)
    std::vector<uint8_t> baseDerivationKey;

    // Key derivation per ANSI X9.24-3
    std::vector<uint8_t> nonReversibleKeyDerivation(const std::vector<uint8_t>& ipek, const std::vector<uint8_t>& ksn);
};
