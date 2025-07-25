#include "DUKPTKeyDerivation.hpp"
#include <stdexcept>
#include <openssl/evp.h>
#include <openssl/aes.h>
#include <cstring>
#include <iostream>

DUKPTKeyDerivation::DUKPTKeyDerivation(const std::vector<uint8_t>& bdk)
    : baseDerivationKey(bdk) {}

std::vector<uint8_t> DUKPTKeyDerivation::deriveKey(const std::vector<uint8_t>& ksn) {
    auto ipek = generateIPEK(ksn);
    return nonReversibleKeyDerivation(ipek, ksn);
}

std::vector<uint8_t> DUKPTKeyDerivation::nonReversibleKeyDerivation(const std::vector<uint8_t>& ipek, const std::vector<uint8_t>& ksn) {
    std::vector<uint8_t> sessionKey(16); // placeholder
    // TODO: Implement non-reversible derivation logic per X9.24-3
    return sessionKey;
}


//****** Integrate AES encryption logic with OpenSSL ******/
std::vector<uint8_t> DUKPTKeyDerivation::aesEncryptECB(const std::vector<uint8_t>& key,const std::vector<uint8_t>& data) {
    std::vector<uint8_t> encrypted(data.size());
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    int len;

    if (!ctx) throw std::runtime_error("Failed to create cipher context");

    if (EVP_EncryptInit_ex(ctx, EVP_aes_128_ecb(), nullptr, key.data(), nullptr) != 1)
        throw std::runtime_error("Encryption init failed");

    EVP_CIPHER_CTX_set_padding(ctx, 0); // DUKPT doesn’t use padding

    if (EVP_EncryptUpdate(ctx, encrypted.data(), &len, data.data(), data.size()) != 1)
        throw std::runtime_error("Encrypt update failed");

    EVP_CIPHER_CTX_free(ctx);
    return encrypted;
}

std::vector<uint8_t> DUKPTKeyDerivation::generateIPEK(const std::vector<uint8_t>& ksn) {
    if (ksn.size() < 10) {
        std::cerr << "Error: KSN must be at least 10 bytes long." << std::endl;
        return {};  // or throw std::invalid_argument("Invalid KSN length")
    }

    std::vector<uint8_t> ipek(16);  // DUKPT IPEK should be 16 bytes

    std::vector<uint8_t> ksnMasked = ksn;
    // Masking steps per ANSI X9.24-3
    ksnMasked[7] &= 0xE0;
    ksnMasked[8] = 0x00;
    ksnMasked[9] = 0x00;

    std::vector<uint8_t> keyLeft(baseDerivationKey.begin(), baseDerivationKey.begin() + 16);
    std::vector<uint8_t> keyRight = keyLeft;

    keyRight[0] ^= 0xC0;

    auto part1 = aesEncryptECB(keyLeft, ksnMasked);
    auto part2 = aesEncryptECB(keyRight, ksnMasked);

    std::copy(part1.begin(), part1.begin() + 8, ipek.begin());
    std::copy(part2.begin(), part2.begin() + 8, ipek.begin() + 8);

    return ipek;
}

