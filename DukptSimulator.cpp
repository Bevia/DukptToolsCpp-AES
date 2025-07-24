#include "DukptSimulator.hpp"
#include <iostream>
#include <regex>
#include "DukptAES.hpp"
#include "AesTerminalSimulator.hpp"
#include "AesHsmSimulator.hpp"
#include "utils/HexUtils.hpp"

void DukptSimulator::runSimulation(const std::string& pin,
                                   const std::string& pan,
                                   AesMode mode) {
    try {
        validatePin(pin);
        validatePan(pan);

        printSimulationHeader(pin, pan);

        std::cout << "DUKPT Format ID: 4 (AES-DUKPT)" << std::endl;
        auto bdk = generateBdk(mode);
        std::cout << "BDK (" << toString(mode) << "): " << HexUtils::bytesToHex(bdk) << std::endl;

        auto ksn = generateInitialKsn();
        std::cout << "Initial KSN (Format 4): " << HexUtils::bytesToHex(ksn) << std::endl;

        auto ipek = DukptAES::deriveIPEK(bdk, ksn);
        std::cout << "IPEK (Derived using AES): " << HexUtils::bytesToHex(ipek) << std::endl << std::endl;

        simulateTransaction(mode, pin, pan, ipek, ksn, bdk);

    } catch (const std::exception& e) {
        std::cerr << "Simulation Error: " << e.what() << std::endl;
    }
}

void DukptSimulator::validatePin(const std::string& pin) {
    if (std::regex_match(pin, std::regex("^[0-9A-Fa-f]{16}$"))) return;

    if (pin.length() < 4 || pin.length() > 12)
        throw std::invalid_argument("PIN must be between 4 and 12 digits");
    if (!std::all_of(pin.begin(), pin.end(), ::isdigit))
        throw std::invalid_argument("PIN must contain only digits");
}

void DukptSimulator::validatePan(const std::string& pan) {
    if (pan.length() < 13)
        throw std::invalid_argument("PAN must be at least 13 digits");
    if (!std::all_of(pan.begin(), pan.end(), ::isdigit))
        throw std::invalid_argument("PAN must contain only digits");
}

void DukptSimulator::printSimulationHeader(const std::string& pin, const std::string& pan) {
    std::cout << "Simulating AES-DUKPT PIN encryption/decryption" << std::endl;
    std::cout << "==============================================" << std::endl;
    std::cout << "PIN: " << maskPin(pin) << std::endl;
    std::cout << "PAN: " << maskPan(pan) << std::endl << std::endl;
}

std::vector<uint8_t> DukptSimulator::generateBdk(AesMode mode) {
    switch (mode) {
        case AesMode::AES_128_ECB:
            return HexUtils::hexToBytes("00112233445566778899AABBCCDDEEFF");
        case AesMode::AES_256_CBC:
            return HexUtils::hexToBytes("00112233445566778899AABBCCDDEEFF0123456789ABCDEFFEDCBA9876543210");
    }
    throw std::invalid_argument("Unknown AES mode");
}

std::vector<uint8_t> DukptSimulator::generateInitialKsn() {
    return HexUtils::hexToBytes("FFFF9876543210E00000000000000000");
}

void DukptSimulator::simulateTransaction(AesMode mode,
                                         const std::string& pin,
                                         const std::string& pan,
                                         const std::vector<uint8_t>& ipek,
                                         const std::vector<uint8_t>& ksn,
                                         const std::vector<uint8_t>& bdk) {

    AesTerminalSimulator terminal(ipek, ksn, true); // testMode = true
    AesHsmSimulator hsm(bdk);

    std::cout << "Terminal encrypting PIN using " << toString(mode) << "..." << std::endl;

    std::vector<uint8_t> encryptedPin;
    std::vector<uint8_t> currentKsn;
    std::vector<uint8_t> iv;

    if (mode == AesMode::AES_128_ECB) {
        std::tie(encryptedPin, currentKsn, std::ignore) = terminal.encryptPin(pin, mode, pan);
    } else {
        std::tie(encryptedPin, currentKsn, iv) = terminal.encryptPin(pin, mode);
    }

    std::cout << "Encrypted PIN block: " << HexUtils::bytesToHex(encryptedPin) << std::endl;
    std::cout << "Current KSN: " << HexUtils::bytesToHex(currentKsn) << std::endl;
    if (!iv.empty()) {
        std::cout << "IV used for CBC: " << HexUtils::bytesToHex(iv) << std::endl;
    }

    std::cout << std::endl << "HSM decrypting PIN using " << toString(mode) << "..." << std::endl;

    std::string decryptedPin;

    if (mode == AesMode::AES_128_ECB) {
        decryptedPin = hsm.decryptPin(mode, encryptedPin, currentKsn, pan);
    } else {
        if (iv.empty()) throw std::runtime_error("IV must not be null for CBC mode");
        decryptedPin = hsm.decryptPin(mode, encryptedPin, currentKsn, iv);
    }

    std::cout << "Decrypted PIN (unmasked): " << decryptedPin << std::endl;

    if (pin != decryptedPin)
        throw std::runtime_error("PIN verification failed!");
}

std::string DukptSimulator::maskPin(const std::string& pin) {
    return std::string(pin.length(), MASK_CHARACTER);
}

std::string DukptSimulator::maskPan(const std::string& pan) {
    if (pan.length() <= 4) return pan;
    return pan.substr(0, 6) + std::string(pan.length() - 10, MASK_CHARACTER) + pan.substr(pan.length() - 4);
}
