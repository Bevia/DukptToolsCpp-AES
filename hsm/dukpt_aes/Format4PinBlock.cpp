#include "Format4PinBlock.hpp"
#include <random>

std::vector<uint8_t> Format4PinBlock::generate(const std::string& pin) {
    std::vector<uint8_t> block(16);
    block[0] = static_cast<uint8_t>(pin.length());

    for (size_t i = 0; i < pin.length(); ++i) {
        block[i + 1] = static_cast<uint8_t>(pin[i] - '0');
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint8_t> dist(1, 255); 

    for (size_t i = pin.length() + 1; i < 16; ++i) {
        block[i] = dist(gen);
    }

    return block;
}
