#pragma once

#include <string>
#include <vector>
#include <tuple>
#include "AesMode.hpp"
#include "utils/HexUtils.hpp"

class DukptSimulator {
public:
    void runSimulation(const std::string& pin = "1234",
                       const std::string& pan = "4532111122223333",
                       AesMode mode = AesMode::AES_256_CBC);

private:
    void validatePin(const std::string& pin);
    void validatePan(const std::string& pan);
    void printSimulationHeader(const std::string& pin, const std::string& pan);

    std::vector<uint8_t> generateBdk(AesMode mode);
    std::vector<uint8_t> generateInitialKsn();
    void simulateTransaction(AesMode mode,
                             const std::string& pin,
                             const std::string& pan,
                             const std::vector<uint8_t>& ipek,
                             const std::vector<uint8_t>& ksn,
                             const std::vector<uint8_t>& bdk);

    std::string maskPin(const std::string& pin);
    std::string maskPan(const std::string& pan);

    static constexpr char MASK_CHARACTER = '*';
};
