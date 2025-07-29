#include "Format0PinBlock.hpp"
#include "DUKPTKeyDerivation.hpp"
#include "DukptHsm.hpp"
#include <iostream>
#include <iomanip>
#include <vector>

inline void printHex(const std::string& label, const std::vector<uint8_t>& data) {
    std::cout << label << ": ";
    for (auto byte : data) {
        std::cout << std::uppercase << std::hex << std::setw(2)
                  << std::setfill('0') << static_cast<int>(byte) << " ";
    }
    std::cout << std::dec << std::endl;
}

int main() {
    std::string pin = "1234";
    std::string pan = "5432109876543210"; // Example PAN
    std::cout << "Clear PIN: " << pin << std::endl;
    std::cout << "PAN: " << pan << std::endl;

    std::vector<uint8_t> bdk = {
        0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF,
        0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10
    };

    std::vector<uint8_t> ksn = {
        0xFF, 0xFF, 0x98, 0x76, 0x54,
        0x32, 0x10, 0xE0, 0x00, 0x00
    };

    // Generate Format 0 PIN block
    auto pinBlock = Format0PinBlock::generate(pin, pan);
    printHex("Format 0 PIN Block (Clear)", pinBlock);
    printHex("KSN", ksn);

    // Derive session key
    DUKPTKeyDerivation dukpt(bdk);
    auto sessionKey = dukpt.deriveKey(ksn);

    // Encrypt PIN block
    auto encryptedBlock = dukpt.tdeaEncryptECB(sessionKey, pinBlock);
    printHex("Encrypted PIN Block", encryptedBlock);

    // Simulate HSM decryption
    DukptHsm hsm;
    auto decryptedBlock = hsm.tdeaDecryptECB(sessionKey, encryptedBlock);
    printHex("Decrypted PIN Block", decryptedBlock);

    // Extract PIN
    std::string extractedPin = Format0PinBlock::extract(decryptedBlock, pan);
    std::cout << "Extracted PIN: " << extractedPin << std::endl;

    return 0;
}
