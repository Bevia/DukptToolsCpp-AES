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

std::string extractPIN(const std::vector<uint8_t> &pinBlock)
    {
        if (pinBlock.empty())
            return "";

        size_t pinLength = pinBlock[0]; // First byte = length
        std::string pin;

        for (size_t i = 1; i <= pinLength; ++i)
        {
            uint8_t digit = pinBlock[i];
            pin += std::to_string(digit);
        }

        return pin;
    }

int main()
{
    std::string pin = "1234";
    std::cout << "PIN: " << pin << std::endl;

    std::vector<uint8_t> bdk = {
        0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF,
        0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10};

    std::vector<uint8_t> ksn = {
        0xFF, 0xFF, 0x98, 0x76, 0x54,
        0x32, 0x10, 0xE0, 0x00, 0x00 // masked bits still intact
    };

    // Generate PIN Block
    auto pinBlock = Format4PinBlock::generate(pin);
    printHex("Format 4 PIN Block (Clear)", pinBlock);

    printHex("KSN", ksn);

    // Derive Session Key
    DUKPTKeyDerivation dukpt(bdk);
    auto sessionKey = dukpt.deriveKey(ksn);

    // Encrypt PIN Block with Session Key
    auto encryptedBlock = dukpt.aesEncryptECB(sessionKey, pinBlock);
    printHex("Encrypted PIN Block", encryptedBlock);

    // In HSM
    auto decryptedBlock = DukptHsm::aesDecryptECB(sessionKey, encryptedBlock);
    printHex("Decrypted PIN Block", decryptedBlock);

    // Extract PIN from decrypted block
    std::string extractedPin = extractPIN(decryptedBlock);
    std::cout << "Extracted PIN: " << pin << std::endl;

    // Log outputs
    std::cout << "Encrypted PIN Block: ";
    for (auto byte : encryptedBlock)
    {
        printf("%02X ", byte);
    }
    std::cout << std::endl;

    return 0;
}
