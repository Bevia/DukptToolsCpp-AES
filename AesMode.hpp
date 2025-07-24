#pragma once

enum class AesMode {
    AES_128_ECB,
    AES_256_CBC
};

inline const char* toString(AesMode mode) {
    switch (mode) {
        case AesMode::AES_128_ECB: return "AES_128_ECB";
        case AesMode::AES_256_CBC: return "AES_256_CBC";
        default: return "Unknown";
    }
}

