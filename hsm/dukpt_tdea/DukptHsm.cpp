#include "DukptHsm.hpp"
#include <openssl/evp.h>
#include <stdexcept>
#include <vector>

std::vector<uint8_t> DukptHsm::tdeaDecryptECB(
    const std::vector<uint8_t>& key,
    const std::vector<uint8_t>& data)
{
    if ((key.size() != 16 && key.size() != 24) || data.size() != 8)
        throw std::invalid_argument("TDEA ECB requires 16- or 24-byte key and 8-byte data");

    std::vector<uint8_t> decrypted(8);
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    int len = 0;

    const EVP_CIPHER* cipher = (key.size() == 16) ? EVP_des_ede() : EVP_des_ede3();

    if (!ctx)
        throw std::runtime_error("Failed to create cipher context");

    if (EVP_DecryptInit_ex(ctx, cipher, nullptr, key.data(), nullptr) != 1)
        throw std::runtime_error("Decryption init failed");

    EVP_CIPHER_CTX_set_padding(ctx, 0);

    if (EVP_DecryptUpdate(ctx, decrypted.data(), &len, data.data(), data.size()) != 1)
        throw std::runtime_error("Decrypt update failed");

    EVP_CIPHER_CTX_free(ctx);
    return decrypted;
}
