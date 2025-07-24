#pragma once

#include <string>
#include <vector>
#include "AesMode.hpp"

class AesTerminalSimulator {
public:
    AesTerminalSimulator(const std::vector<uint8_t>& ipek,
                         const std::vector<uint8_t>& ksn,
                         bool testMode = false);

    std::tuple<std::vector<uint8_t>, std::vector<uint8_t>, std::vector<uint8_t>>
    encryptPin(const std::string& pin,
               AesMode mode,
               const std::string& pan = "",
               const std::vector<uint8_t>* ivOverride = nullptr);

private:
    std::vector<uint8_t> deriveSessionKey(const std::vector<uint8_t>& ipek,
                                          const std::vector<uint8_t>& ksn);

    std::vector<uint8_t> generatePinBlock(const std::string& pin);

    std::vector<uint8_t> aesEncryptEcb(const std::vector<uint8_t>& key,
                                       const std::vector<uint8_t>& data);

    std::vector<uint8_t> aesEncryptCbc(const std::vector<uint8_t>& key,
                                       const std::vector<uint8_t>& iv,
                                       const std::vector<uint8_t>& data);

    std::vector<uint8_t> ipek_;
    std::vector<uint8_t> ksn_;
    bool testMode_;
};
