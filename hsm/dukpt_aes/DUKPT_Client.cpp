#include "Format4PinBlock.hpp"
#include "DUKPTKeyDerivation.hpp"
#include <iostream>

int main() {
    std::string pin = "1234";

    std::vector<uint8_t> bdk = {
    0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF,
    0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10
};

    std::vector<uint8_t> ksn = {
    0xFF, 0xFF, 0x98, 0x76, 0x54,
    0x32, 0x10, 0xE0, 0x00, 0x00  // masked bits still intact
};


    // Generate PIN Block
    auto pinBlock = Format4PinBlock::generate(pin);

    // Derive Session Key
    DUKPTKeyDerivation dukpt(bdk);
    auto sessionKey = dukpt.deriveKey(ksn);

    // Encrypt PIN Block with Session Key
    auto encryptedBlock = dukpt.aesEncryptECB(sessionKey, pinBlock);

    // Log outputs
    std::cout << "Encrypted PIN Block: ";
    for (auto byte : encryptedBlock) {
        printf("%02X ", byte);
    }
    std::cout << std::endl;

    return 0;
}
