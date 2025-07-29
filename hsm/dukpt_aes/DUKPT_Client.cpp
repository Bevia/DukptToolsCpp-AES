#include "Format4PinBlock.hpp"
#include "DUKPTKeyDerivation.hpp"
#include <iostream>
#include <iomanip>
#include <vector>
#include "DukptHsm.hpp"

inline void printHex(const std::string &label, const std::vector<uint8_t> &data)
{
    std::cout << label << ": ";
    for (auto byte : data)
    {
        std::cout << std::uppercase << std::hex << std::setw(2)
                  << std::setfill('0') << static_cast<int>(byte) << " ";
    }
    std::cout << std::dec << std::endl;
}

std::string extractPIN(const std::vector<uint8_t> &decryptedBlock, size_t pinLength)
{
    if (decryptedBlock.size() < pinLength)
        return "";

    std::string pin;
    for (size_t i = 0; i < pinLength; ++i)
    {
        pin += std::to_string(decryptedBlock[i]);
    }

    return pin;
}

int main()
{
    std::string pin = "1234";
    std::string pan = "1234567890123456"; // PAN must be 12 rightmost digits excluding check digit
    std::cout << "PIN: " << pin << std::endl;

    std::vector<uint8_t> bdk = {
        0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF,
        0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10};

    std::vector<uint8_t> ksn = {
        0xFF, 0xFF, 0x98, 0x76, 0x54,
        0x32, 0x10, 0xE0, 0x00, 0x00 // masked bits still intact
    };

    printHex("KSN", ksn);

    // Derive session key
    DUKPTKeyDerivation dukpt(bdk);
    auto sessionKey = dukpt.deriveKey(ksn);

    // Generate Format 4 PIN block (PIN + PAN combined, padded to 16 bytes)
    auto pinBlock = Format4PinBlock::generate(pin, pan);
    printHex("Format 4 PIN Block (Clear)", pinBlock);

    // Encrypt PIN Block with session key (AES-ECB or CBC if preferred)
    auto encryptedBlock = dukpt.aesEncryptECB(sessionKey, pinBlock);
    printHex("Encrypted PIN Block", encryptedBlock);

    // Simulate HSM decryption
    auto decryptedBlock = DukptHsm::aesDecryptECB(sessionKey, encryptedBlock);
    printHex("Decrypted PIN Block", decryptedBlock);

    // Extract PIN from decrypted block
    std::string extractedPin = extractPIN(decryptedBlock, pin.length());
    std::cout << "Extracted PIN: " << extractedPin << std::endl;

    return 0;
}