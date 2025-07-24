brew install openssl

g++ -std=c++17 \
    HSMClient.cpp DukptSimulator.cpp \
    AesTerminalSimulator.cpp AesHsmSimulator.cpp \
    DukptAES.cpp utils/HexUtils.cpp \
    testvectors/DukptTestVectors.cpp \
    -I. -I/opt/homebrew/opt/openssl/include \
    -L/opt/homebrew/opt/openssl/lib \
    -lssl -lcrypto -Wno-deprecated-declarations \
    -o HSMClient


./HSMClient
