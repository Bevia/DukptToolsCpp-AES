#include "HexUtils.hpp"
#include <stdexcept>
#include <sstream>
#include <iomanip>

std::vector<uint8_t> HexUtils::hexToBytes(const std::string& hex) {
    if (hex.length() % 2 != 0) {
        throw std::invalid_argument("Hex string must have an even length");
    }

    std::vector<uint8_t> bytes;
    bytes.reserve(hex.length() / 2);

    for (size_t i = 0; i < hex.length(); i += 2) {
        std::string byteStr = hex.substr(i, 2);
        uint8_t byte = static_cast<uint8_t>(std::stoi(byteStr, nullptr, 16));
        bytes.push_back(byte);
    }

    return bytes;
}

std::string HexUtils::bytesToHex(const std::vector<uint8_t>& bytes) {
    std::ostringstream oss;

    for (auto b : bytes) {
        oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(b);
    }

    return oss.str();
}
