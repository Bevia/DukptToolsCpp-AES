#include "AesTerminalSimulator.hpp"
#include "DukptAES.hpp"
#include <openssl/evp.h>
#include <openssl/aes.h>
#include <random>
#include <stdexcept>

AesTerminalSimulator::AesTerminalSimulator(const std::vector<uint8_t>& ipek,
                                           const std::vector<uint8_t>& ksn,
                                           bool testMode)
    : ipek_(ipek), ksn_(ksn), testMode_(testMode) {}

std::tuple<std::vector<uint8_t>, std::vector<uint8_t>, std::vector<uint8_t>>
AesTerminalSimulator::encryptPin(const std::string& pin,
                                 AesMode mode,
                                 const std::string& /*pan*/,
                                 const std::vector<uint8_t>* ivOverride) {
    auto sessionKey = deriveSessionKey(ipek_, ksn_);

    std::vector<uint8_t> iv = ivOverride ? *ivOverride :
        (testMode_ ? std::vector<uint8_t>(16, 0) : [] {
            std::vector<uint8_t> randomIv(16);
            std::random_device rd;
            std::mt19937 rng(rd());
            std::uniform_int_distribution<int> dist(0, 255);
            for (auto& b : randomIv) b = static_cast<uint8_t>(dist(rng));
            return randomIv;
        }());

    auto pinBlock = generatePinBlock(pin);

    switch (mode) {
        case AesMode::AES_128_ECB: {
            auto encrypted = aesEncryptEcb(sessionKey, pinBlock);
            return {encrypted, ksn_, {}};
        }
        case AesMode::AES_256_CBC: {
            auto encrypted = aesEncryptCbc(sessionKey, iv, pinBlock);
            return {encrypted, ksn_, iv};
        }
        default:
            throw std::invalid_argument("Unknown AES mode");
    }
}

std::vector<uint8_t> AesTerminalSimulator::deriveSessionKey(const std::vector<uint8_t>& ipek,
                                                            const std::vector<uint8_t>& ksn) {
    return DukptAES::deriveIPEK(ipek, ksn);
}

std::vector<uint8_t> AesTerminalSimulator::generatePinBlock(const std::string& pin) {
    std::vector<uint8_t> block(16, 0);
    block[0] = 0x04;
    block[1] = static_cast<uint8_t>(pin.length());

    for (size_t i = 0; i < 12; ++i) {
        if (i < pin.length()) {
            block[i + 2] = static_cast<uint8_t>(pin[i] - '0');
        } else {
            block[i + 2] = 0x0F;
        }
    }

    if (testMode_) {
        block[14] = 0x00;
        block[15] = 0x00;
    } else {
        std::random_device rd;
        std::mt19937 rng(rd());
        std::uniform_int_distribution<int> dist(0, 255);
        block[14] = static_cast<uint8_t>(dist(rng));
        block[15] = static_cast<uint8_t>(dist(rng));
    }

    return block;
}

std::vector<uint8_t> AesTerminalSimulator::aesEncryptEcb(const std::vector<uint8_t>& key,
                                                         const std::vector<uint8_t>& data) {
    std::vector<uint8_t> output(data.size());

    AES_KEY aesKey;
    AES_set_encrypt_key(key.data(), key.size() * 8, &aesKey);
    AES_ecb_encrypt(data.data(), output.data(), &aesKey, AES_ENCRYPT);

    return output;
}

std::vector<uint8_t> AesTerminalSimulator::aesEncryptCbc(const std::vector<uint8_t>& key,
                                                         const std::vector<uint8_t>& iv,
                                                         const std::vector<uint8_t>& data) {
    std::vector<uint8_t> output(data.size() + AES_BLOCK_SIZE); // PKCS#7 padded

    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    const EVP_CIPHER* cipher = key.size() == 16 ? EVP_aes_128_cbc() : EVP_aes_256_cbc();

    EVP_EncryptInit_ex(ctx, cipher, nullptr, key.data(), iv.data());

    int outLen1 = 0;
    EVP_EncryptUpdate(ctx, output.data(), &outLen1, data.data(), data.size());

    int outLen2 = 0;
    EVP_EncryptFinal_ex(ctx, output.data() + outLen1, &outLen2);

    output.resize(outLen1 + outLen2);
    EVP_CIPHER_CTX_free(ctx);

    return output;
}
