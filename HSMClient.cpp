#include <iostream>
#include "DukptSimulator.hpp"
#include "testvectors/DukptTestVectors.hpp"
#include "DukptAES.hpp"
#include "AesTerminalSimulator.hpp"
#include "AesHsmSimulator.hpp"
#include "utils/HexUtils.hpp"

int main() {
    // Initialize simulator
    DukptSimulator simulator;

    // Run simulation with chosen parameters
    simulator.runSimulation(
        "1234",                                 // pin
        "4532111122223333",                     // pan
        AesMode::AES_128_ECB                    // or AES_256_CBC
    );

    // Validate test vectors
    bool allPassed = DukptTestVectors::validateAll();
    std::cout << (allPassed ? "✅ All tests passed!" : "❌ Some tests failed.") << std::endl;

    return allPassed ? 0 : 1;


}
