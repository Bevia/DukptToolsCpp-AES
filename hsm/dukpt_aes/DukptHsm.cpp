#include "DukptHsm.hpp"
#include <openssl/evp.h>
#include <stdexcept>
#include <vector>

std::vector<uint8_t> DukptHsm::aesDecryptECB(
    const std::vector<uint8_t>& key,
    const std::vector<uint8_t>& data)
{
    if (key.size() != 16 || data.size() != 16)
        throw std::invalid_argument("AES ECB requires 16-byte key and data");

    std::vector<uint8_t> decrypted(16);
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    int len = 0;

    if (!ctx)
        throw std::runtime_error("Failed to create cipher context");

    if (EVP_DecryptInit_ex(ctx, EVP_aes_128_ecb(), nullptr, key.data(), nullptr) != 1)
        throw std::runtime_error("Decryption init failed");

    EVP_CIPHER_CTX_set_padding(ctx, 0);

    if (EVP_DecryptUpdate(ctx, decrypted.data(), &len, data.data(), data.size()) != 1)
        throw std::runtime_error("Decrypt update failed");

    EVP_CIPHER_CTX_free(ctx);
    return decrypted;
}
