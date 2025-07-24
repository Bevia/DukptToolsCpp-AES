```markdown
# 🔐 Dukpt AES Simulator (C++)

A lightweight, modular cryptographic simulator written in C++ that mimics HSM behavior using AES-based DUKPT key derivation and PIN block encryption/decryption.

Inspired by the Kotlin version — translated and expanded for high-performance C++ environments.

---

## 🧩 Features

- AES-128-ECB and AES-256-CBC modes
- DUKPT IPEK and session key derivation using AES-CMAC
- Hex encoding/decoding utilities
- PIN block generation and parsing
- Simulator classes for terminal (encrypt) and HSM (decrypt)
- Built-in test vector validation

---

## 🚀 Getting Started

### 🔧 Prerequisites

- C++17 compatible compiler (`g++`, `clang++`, MSVC)
- OpenSSL 3.x (`libssl`, `libcrypto`) with development headers
- CMake (optional but recommended)

### 💻 Compilation (Manual)

```bash
g++ -std=c++17 \
    HSMClient.cpp DukptSimulator.cpp \
    AesTerminalSimulator.cpp AesHsmSimulator.cpp \
    DukptAES.cpp HexUtils.cpp \
    -I/opt/homebrew/opt/openssl/include \
    -L/opt/homebrew/opt/openssl/lib \
    -lssl -lcrypto -Wno-deprecated-declarations \
    -o HSMClient
```

Adjust OpenSSL paths for your platform.

---

## 📂 Project Structure

```text
├── AesMode.hpp                 # AES mode enum
├── AesTerminalSimulator.*      # Encrypts PIN using session key
├── AesHsmSimulator.*           # Decrypts PIN block, extracts PIN
├── DukptAES.*                  # AES-CMAC based key derivation
├── DukptTestVectors.*          # Static test vectors (validation)
├── DukptSimulator.*            # Orchestrates PIN lifecycle
├── HexUtils.*                  # Hex <-> byte utilities
├── HSMClient.cpp               # Entry point / main driver
└── README.md                   # You're reading this!
```

---

## 🔁 Example Usage

```cpp
int main() {
    DukptSimulator simulator;
    simulator.runSimulation(
        "1234",                      // PIN
        "4532111122223333",          // PAN
        AesMode::AES_128_ECB         // or AES_256_CBC
    );

    bool allPassed = DukptTestVectors::validateAll();
    std::cout << (allPassed ? "✅ All tests passed!" : "❌ Some tests failed.") << std::endl;

    return allPassed ? 0 : 1;
}
```

---

## ✅ Test Vectors

Includes built-in hardcoded test vector:

```json
{
  "bdk": "0123456789ABCDEFFEDCBA9876543210",
  "ksn": "FFFF9876543210E00000000000000000",
  "expectedIpek": "9E9A8F275E612386E3F22C10CEE59CE9",
  "pin": "1234",
  "pan": "4532111122223333",
  "expectedEncryptedPin": "94cad288c71da94aaf0b902ba30b67667db8541728901ccabf9a07e96944fbab"
}
```

Later you can wire in JSON-based dynamic test sets.

---

## 📌 TODOs

- [ ] Implement ECB PIN block parsing
- [ ] Add vector JSON loader
- [ ] CMake config + unit tests
- [ ] Export encrypted PIN block to external systems
- [ ] CLI options (e.g., `--validate`, `--mode`, `--pin`)

---

## 🧠 Credits

Crafted with precision by [Vincent] — ported from a Kotlin implementation and expanded in collaboration with Microsoft Copilot. Built for clarity, security, and testability.

---

## ⚖️ License

This project is licensed under the MIT License.

---

### MIT License

Copyright © 2025 Vincent

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the “Software”), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
