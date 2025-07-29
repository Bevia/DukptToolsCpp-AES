#include "DUKPTKeyDerivation.hpp"
#include <stdexcept>
#include <openssl/evp.h>
#include <openssl/des.h>
#include <cstring>
#include <vector>
#include <iostream>
#include <iomanip> // for std::setw, std::setfill, std::hex

DUKPTKeyDerivation::DUKPTKeyDerivation(const std::vector<uint8_t>& bdk)
    : baseDerivationKey(bdk) {}

std::vector<uint8_t> DUKPTKeyDerivation::deriveKey(const std::vector<uint8_t>& ksn) {
    auto ipek = generateIPEK(ksn);
    return nonReversibleKeyDerivation(ipek, ksn);
}

// Triple DES ECB encryption
std::vector<uint8_t> DUKPTKeyDerivation::tdeaEncryptECB(const std::vector<uint8_t>& key, const std::vector<uint8_t>& data) {
    if ((key.size() != 16 && key.size() != 24) || data.size() != 8)
        throw std::invalid_argument("Triple DES requires 16- or 24-byte key and 8-byte data");

    std::vector<uint8_t> encrypted(8);
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    int len = 0;

    const EVP_CIPHER* cipher = (key.size() == 16) ? EVP_des_ede() : EVP_des_ede3();

    if (!ctx)
        throw std::runtime_error("Failed to create cipher context");

    if (EVP_EncryptInit_ex(ctx, cipher, nullptr, key.data(), nullptr) != 1)
        throw std::runtime_error("Encryption init failed");

    EVP_CIPHER_CTX_set_padding(ctx, 0);

    if (EVP_EncryptUpdate(ctx, encrypted.data(), &len, data.data(), data.size()) != 1)
        throw std::runtime_error("Encrypt update failed");

    EVP_CIPHER_CTX_free(ctx);
    return encrypted;
}

// IPEK Generation per ANSI X9.24-3 using TDEA
std::vector<uint8_t> DUKPTKeyDerivation::generateIPEK(const std::vector<uint8_t>& ksn) {
    if (ksn.size() < 10) {
        std::cerr << "Error: KSN must be at least 10 bytes long." << std::endl;
        return {};
    }

    std::vector<uint8_t> ipek(16);
    std::vector<uint8_t> ksnMasked = ksn;

    // Mask the counter in the KSN
    ksnMasked[7] &= 0xE0;
    ksnMasked[8] = 0x00;
    ksnMasked[9] = 0x00;

    std::vector<uint8_t> ksnBlock(ksnMasked.begin(), ksnMasked.begin() + 8);
    std::vector<uint8_t> keyLeft(baseDerivationKey.begin(), baseDerivationKey.begin() + 16);
    std::vector<uint8_t> keyRight = keyLeft;

    // XOR first byte with 0xC0 (X9.24-3 spec)
    keyRight[0] ^= 0xC0;

    auto part1 = tdeaEncryptECB(keyLeft, ksnBlock);
    auto part2 = tdeaEncryptECB(keyRight, ksnBlock);

    std::copy(part1.begin(), part1.end(), ipek.begin());
    std::copy(part2.begin(), part2.end(), ipek.begin() + 8);

    printHex("Derived IPEK", ipek);
    return ipek;
}

// Placeholder for session key derivation
std::vector<uint8_t> DUKPTKeyDerivation::nonReversibleKeyDerivation(const std::vector<uint8_t>& ipek, const std::vector<uint8_t>& ksn) {
    std::vector<uint8_t> sessionKey(16);
    // TODO: Implement actual derivation using key masks and transaction counter
    return sessionKey;
}

// Utility for hex output
inline void printHex(const std::string& label, const std::vector<uint8_t>& data) {
    std::cout << label << ": ";
    for (auto byte : data) {
        std::cout << std::uppercase << std::hex << std::setw(2)
                  << std::setfill('0') << static_cast<int>(byte) << " ";
    }
    std::cout << std::dec << std::endl;
}
