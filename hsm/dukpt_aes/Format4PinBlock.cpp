#include "Format4PinBlock.hpp"
#include <iomanip>
#include <sstream>
#include <cctype>
#include <stdexcept>

std::vector<uint8_t> Format4PinBlock::generate(const std::string &pin, const std::string &pan)
{
    // Validate PIN
    if (pin.empty() || pin.length() > 12)
        throw std::invalid_argument("Invalid PIN length. Must be 1–12 digits.");
    for (char c : pin)
        if (!std::isdigit(c))
            throw std::invalid_argument("PIN must contain only digits.");

    // Validate PAN
    if (pan.length() < 13)
        throw std::invalid_argument("Invalid PAN length. Must be at least 13 digits.");
    for (char c : pan)
        if (!std::isdigit(c))
            throw std::invalid_argument("PAN must contain only digits.");

    // Extract 12 digits from PAN: rightmost 13 digits, excluding check digit
    std::string pan12 = pan.substr(pan.length() - 13, 12);

    std::vector<uint8_t> pinBlock;

    // Add PIN digits
    for (char c : pin)
        pinBlock.push_back(static_cast<uint8_t>(c - '0'));

    // Pad PIN digits with 0xFF until we have 4 bytes
    while (pinBlock.size() < 4)
        pinBlock.push_back(0xFF);

    // Add PAN12 digits (12 bytes)
    for (char c : pan12)
        pinBlock.push_back(static_cast<uint8_t>(c - '0'));

    /*
    Format 4 is defined as:
        A 16-byte block that includes:
        8 bytes of PIN (format + PIN digits + padding)

        (usually the 12 rightmost digits, padded)
    */
   
    /***** Ensure final block is 16 bytes *****/ 
    if (pinBlock.size() != 16)
        throw std::runtime_error("Generated PIN block must be exactly 16 bytes");

    return pinBlock;
}