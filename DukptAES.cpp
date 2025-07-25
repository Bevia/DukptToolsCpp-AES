#include "DukptAES.hpp"
#include <openssl/evp.h>
#include <openssl/core_names.h>
#include <cstring>
#include <stdexcept>
#include "AesMode.hpp" // If 'toString' is here

std::vector<uint8_t> DukptAES::deriveIPEK(const std::vector<uint8_t> &bdk,
                                          const std::vector<uint8_t> &ksn)
{
    if (ksn.size() < 8)
        throw std::invalid_argument("KSN must be at least 8 bytes");

    std::vector<uint8_t> diversificationData(ksn.begin(), ksn.begin() + 8);
    return aesCmac(bdk, diversificationData);
}

std::vector<uint8_t> DukptAES::aesCmac(const std::vector<uint8_t> &key,
                                       const std::vector<uint8_t> &data)
{
    EVP_MAC *mac = EVP_MAC_fetch(nullptr, "CMAC", nullptr);
    if (!mac)
        throw std::runtime_error("EVP_MAC_fetch failed");

    EVP_MAC_CTX *ctx = EVP_MAC_CTX_new(mac);
    if (!ctx)
    {
        EVP_MAC_free(mac);
        throw std::runtime_error("EVP_MAC_CTX_new failed");
    }

    //@TODO: This makes AES-256-CBC fail with ---> Simulation Error: EVP_MAC_init failed
    /*
     OSSL_PARAM params[] = {
         OSSL_PARAM_construct_utf8_string("cipher", (char*)"AES-128-CBC", strlen("AES-128-CBC")),
         OSSL_PARAM_construct_end()
     };
    */

    const char *cipherName = nullptr;

    if (key.size() == 16)
    {
        cipherName = "AES-128-CBC";
    }
    else if (key.size() == 32)
    {
        cipherName = "AES-256-CBC";
    }
    else
    {
        throw std::invalid_argument("Unsupported key length for AES-CMAC");
    }

    OSSL_PARAM params[] = {
        OSSL_PARAM_construct_utf8_string("cipher", const_cast<char *>(cipherName), strlen(cipherName)),
        OSSL_PARAM_construct_end()};

    if (!EVP_MAC_init(ctx, key.data(), key.size(), params))
        throw std::runtime_error("EVP_MAC_init failed");

    if (!EVP_MAC_update(ctx, data.data(), data.size()))
        throw std::runtime_error("EVP_MAC_update failed");

    size_t macLen = 0;
    if (!EVP_MAC_final(ctx, nullptr, &macLen, 0))
        throw std::runtime_error("EVP_MAC_final (length fetch) failed");

    std::vector<uint8_t> result(macLen);
    if (!EVP_MAC_final(ctx, result.data(), &macLen, macLen))
        throw std::runtime_error("EVP_MAC_final (MAC output) failed");

    result.resize(macLen);

    EVP_MAC_CTX_free(ctx);
    EVP_MAC_free(mac);

    return result;
}
