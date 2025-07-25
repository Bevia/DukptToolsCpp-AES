#include <openssl/evp.h>
#include <openssl/des.h>
#include <iostream>
#include <iomanip>
#include <cstring>

// Simulated: Encrypt with AES to derive session key
void deriveSessionKey(const unsigned char* bdk, const unsigned char* ksn, unsigned char* sessionKey) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    int len;
    
    EVP_EncryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, bdk, NULL);
    EVP_EncryptUpdate(ctx, sessionKey, &len, ksn, 8); // Simplified input
    EVP_EncryptFinal_ex(ctx, sessionKey + len, &len);

    EVP_CIPHER_CTX_free(ctx);
}

int main() {
    // Base Derivation Key (usually injected)
    unsigned char bdk[16] = {
        0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF,
        0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10
    };

    // Simulated KSN for terminal + transaction
    unsigned char ksn[8] = {
        0xFF, 0xFF, 0x98, 0x76, 0x54, 0x32, 0x10, 0x00
    };

    unsigned char sessionKey[16];

    deriveSessionKey(bdk, ksn, sessionKey);

    std::cout << "Derived Session Key: ";
    for (int i = 0; i < 16; ++i) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)sessionKey[i];
    }
    std::cout << std::endl;

    return 0;
}
