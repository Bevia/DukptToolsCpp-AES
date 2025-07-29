#include "Format0PinBlock.hpp"
#include <vector>
#include <stdexcept>
#include <cstdint>
#include <iostream>

std::vector<uint8_t> Format0PinBlock::generate(const std::string &pin, const std::string &pan)
{
    if (pin.length() < 4 || pin.length() > 12)
        throw std::invalid_argument("PIN length must be between 4 and 12 digits");

    if (pan.length() < 13)
        throw std::invalid_argument("PAN must be at least 13 digits");

    std::vector<uint8_t> pinBlock(8, 0x00);

    // Build PIN block
    std::vector<uint8_t> pinField(8, 0xFF); // filler nibbles
    // pinField[0] = (0x00 << 4) | (pin.length() & 0x0F); // format 0, length N

    pinField[0] = static_cast<uint8_t>(pin.length() & 0x0F); // keeps high nibble zero

    // pinField[0] = 0x00 | pin.length(); // format: 0N

    for (size_t i = 0; i < pin.length(); ++i)
    {
        size_t bytePos = (i / 2) + 1; // Start at pinField[1]
        if (i % 2 == 0)
            pinField[bytePos] = (pin[i] - '0') << 4;
        else
            pinField[bytePos] |= (pin[i] - '0');
    }

    // Extract 12 right-most digits (excluding last check digit)
    std::string pan12 = pan.substr(pan.length() - 13, 12);

    std::vector<uint8_t> panField(8, 0x00);
    for (size_t i = 0; i < pan12.length(); ++i)
    {
        size_t bytePos = i / 2;
        if (i % 2 == 0)
            panField[bytePos] = (pan12[i] - '0') << 4;
        else
            panField[bytePos] |= (pan12[i] - '0');
    }

    // XOR PIN block with PAN field
    for (size_t i = 0; i < 8; ++i)
        pinBlock[i] = pinField[i] ^ panField[i];

    // pinField[0] = static_cast<uint8_t>(pin.length() & 0x0F); // format 0, length N
    std::cout << "PIN Field[0]: 0x" << std::hex << static_cast<int>(pinField[0]) << std::endl;

    return pinBlock;
}

std::string Format0PinBlock::extract(const std::vector<uint8_t> &decryptedBlock, const std::string &pan)
{
    // Reconstruct PAN field
    std::string pan12 = pan.substr(pan.length() - 13, 12);
    std::vector<uint8_t> panField(8, 0x00);

    for (size_t i = 0; i < pan12.length(); ++i)
    {
        size_t bytePos = i / 2;
        if (i % 2 == 0)
            panField[bytePos] = (pan12[i] - '0') << 4;
        else
            panField[bytePos] |= (pan12[i] - '0');
    }

    // XOR decrypted block with PAN field to recover PIN field
    std::vector<uint8_t> pinField(8);
    for (size_t i = 0; i < 8; ++i)
        pinField[i] = decryptedBlock[i] ^ panField[i];

    // Extract PIN length from low nibble of byte 0
    size_t pinLength = pinField[0] & 0x0F;
    std::string pin;

    size_t digitIndex = 0;
    for (size_t i = 1; digitIndex < pinLength; ++i)
    {
        uint8_t byte = pinField[i];
        pin += std::to_string((byte & 0xF0) >> 4);
        ++digitIndex;
        if (digitIndex < pinLength)
            pin += std::to_string(byte & 0x0F);
        ++digitIndex;
    }

    return pin;
}
