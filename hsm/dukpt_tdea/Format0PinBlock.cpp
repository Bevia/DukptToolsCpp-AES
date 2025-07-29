#include "Format0PinBlock.hpp"
#include <vector>
#include <stdexcept>
#include <cstdint>

std::vector<uint8_t> Format0PinBlock::generate(const std::string& pin, const std::string& pan) {
    if (pin.length() < 4 || pin.length() > 12)
        throw std::invalid_argument("PIN length must be between 4 and 12 digits");

    if (pan.length() < 13)
        throw std::invalid_argument("PAN must be at least 13 digits");

    std::vector<uint8_t> pinBlock(8, 0x00);

    // Build PIN block
    std::vector<uint8_t> pinField(8, 0xFF); // filler nibbles
    pinField[0] = 0x00 | pin.length(); // format: 0N

    for (size_t i = 0; i < pin.length(); ++i) {
        size_t bytePos = (i + 1) / 2;
        if ((i + 1) % 2 == 1)
            pinField[bytePos] = (pin[i] - '0') << 4;
        else
            pinField[bytePos] |= (pin[i] - '0');
    }

    // Extract 12 right-most digits (excluding last check digit)
    std::string pan12 = pan.substr(pan.length() - 13, 12);

    std::vector<uint8_t> panField(8, 0x00);
    for (size_t i = 0; i < pan12.length(); ++i) {
        size_t bytePos = i / 2;
        if (i % 2 == 0)
            panField[bytePos] = (pan12[i] - '0') << 4;
        else
            panField[bytePos] |= (pan12[i] - '0');
    }

    // XOR PIN block with PAN field
    for (size_t i = 0; i < 8; ++i)
        pinBlock[i] = pinField[i] ^ panField[i];

    return pinBlock;
}
