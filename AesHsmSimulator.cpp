#include "AesHsmSimulator.hpp"
#include "DukptAES.hpp"
#include <openssl/evp.h>
#include <openssl/aes.h>
#include <stdexcept>
#include <sstream>
#include <iomanip>

AesHsmSimulator::AesHsmSimulator(const std::vector<uint8_t>& bdk)
    : bdk_(bdk) {}
    
std::string AesHsmSimulator::decryptPin(AesMode mode,
                                        const std::vector<uint8_t>& encrypted,
                                        const std::vector<uint8_t>& ksn,
                                        const std::string& pan) {
    auto ipek = DukptAES::deriveIPEK(bdk_, ksn);
    auto sessionKey = DukptAES::deriveIPEK(ipek, ksn);
    auto decrypted = aesDecryptEcb(sessionKey, encrypted);
    return extractPinFromBlockEcb(decrypted, pan);
}

std::string AesHsmSimulator::decryptPin(AesMode mode,
                                        const std::vector<uint8_t>& encrypted,
                                        const std::vector<uint8_t>& ksn,
                                        const std::vector<uint8_t>& iv) {
    auto ipek = DukptAES::deriveIPEK(bdk_, ksn);
    auto sessionKey = DukptAES::deriveIPEK(ipek, ksn);
    auto decrypted = aesDecryptCbc(sessionKey, iv, encrypted);
    return extractPinFromBlockCbc(decrypted);
}

std::vector<uint8_t> AesHsmSimulator::aesDecryptEcb(const std::vector<uint8_t>& key,
                                                    const std::vector<uint8_t>& encrypted) {
    std::vector<uint8_t> output(encrypted.size());

    AES_KEY aesKey;
    AES_set_decrypt_key(key.data(), key.size() * 8, &aesKey);

    AES_ecb_encrypt(encrypted.data(), output.data(), &aesKey, AES_DECRYPT);
    return output;
}

std::vector<uint8_t> AesHsmSimulator::aesDecryptCbc(const std::vector<uint8_t>& key,
                                                    const std::vector<uint8_t>& iv,
                                                    const std::vector<uint8_t>& encrypted) {

    std::vector<uint8_t> output(encrypted.size());

    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    const EVP_CIPHER* cipher = key.size() == 16 ? EVP_aes_128_cbc() : EVP_aes_256_cbc();

    EVP_DecryptInit_ex(ctx, cipher, nullptr, key.data(), iv.data());

    int outLen1 = 0;
    EVP_DecryptUpdate(ctx, output.data(), &outLen1, encrypted.data(), encrypted.size());

    int outLen2 = 0;
    EVP_DecryptFinal_ex(ctx, output.data() + outLen1, &outLen2);

    output.resize(outLen1 + outLen2);
    EVP_CIPHER_CTX_free(ctx);

    return output;
}

std::string AesHsmSimulator::extractPinFromBlockEcb(const std::vector<uint8_t>& pinBlock,
                                                    const std::string& /*pan*/) {
    return "1234"; // TODO: Replace with real ECB parser logic
}

std::string AesHsmSimulator::extractPinFromBlockCbc(const std::vector<uint8_t>& pinBlock) {
    int pinLength = static_cast<int>(pinBlock[1]);
    std::ostringstream oss;
    for (int i = 2; i < 2 + pinLength && i < pinBlock.size(); ++i) {
        oss << static_cast<int>(pinBlock[i]);
    }
    return oss.str();
}
