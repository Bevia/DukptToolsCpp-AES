#pragma once

#include <string>

struct DukptTestVector {
    std::string bdk;
    std::string ksn;
    std::string expectedIpek;
    std::string pin;
    std::string pan;
    std::string expectedEncryptedPin;
};
