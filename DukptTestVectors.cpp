#include "DukptTestVectors.hpp"
#include "HexUtils.hpp"
#include "DukptAES.hpp"
#include "AesTerminalSimulator.hpp"
#include <iostream>

bool DukptTestVectors::validateAll() {
    bool allPassed = true;

    // ✅ Hardcoded test vector
    DukptTestVector vector{
        "0123456789ABCDEFFEDCBA9876543210",                  // bdk
        "FFFF9876543210E00000000000000000",                  // ksn
        "9E9A8F275E612386E3F22C10CEE59CE9",                  // expectedIPEK
        "1234",                                              // pin
        "4532111122223333",                                  // pan
        "94cad288c71da94aaf0b902ba30b67667db8541728901ccabf9a07e96944fbab" // expectedEncryptedPin
    };

    std::cout << "🔎 Validating hardcoded test vector..." << std::endl;

    auto bdk = HexUtils::hexToBytes(vector.bdk);
    auto ksn = HexUtils::hexToBytes(vector.ksn);
    auto expectedIpek = HexUtils::hexToBytes(vector.expectedIpek);
    auto expectedEncryptedPin = HexUtils::hexToBytes(vector.expectedEncryptedPin);

    auto computedIpek = DukptAES::deriveIPEK(bdk, ksn);
    bool ipekMatch = (computedIpek == expectedIpek);

    if (!ipekMatch) {
        std::cout << "❌ IPEK mismatch!\nExpected: " << vector.expectedIpek
                  << "\nActual:   " << HexUtils::bytesToHex(computedIpek) << std::endl;
    } else {
        std::cout << "✅ IPEK verified." << std::endl;
    }

    AesTerminalSimulator terminal(computedIpek, ksn, true);
    std::vector<uint8_t> fixedIV(16, 0);
    auto [encrypted, _, __] = terminal.encryptPin(vector.pin, AesMode::AES_256_CBC, "", &fixedIV);

    bool encryptionMatch = (encrypted == expectedEncryptedPin);

    if (!encryptionMatch) {
        std::cout << "❌ Encrypted PIN mismatch!\nExpected: " << vector.expectedEncryptedPin
                  << "\nActual:   " << HexUtils::bytesToHex(encrypted) << std::endl;
        allPassed = false;
    } else {
        std::cout << "✅ Encrypted PIN verified." << std::endl;
    }

    return allPassed;
}
