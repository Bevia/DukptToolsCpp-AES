g++ -std=c++17 \
./hsm/dukpt_aes/DUKPT_Client.cpp \
./hsm/dukpt_aes/DUKPTKeyDerivation.cpp \
./hsm/dukpt_aes/Format4PinBlock.cpp \
-I. -I/opt/homebrew/opt/openssl/include \
-L/opt/homebrew/opt/openssl/lib \
 -lssl -lcrypto -Wno-deprecated-declarations \
-o DUKPT_Client

./DUKPT_Client


g++ -std=c++17 -g -Wall -I. -I/opt/homebrew/opt/openssl/include \
./hsm/dukpt_aes/DUKPT_Client.cpp \
./hsm/dukpt_aes/DUKPTKeyDerivation.cpp \
./hsm/dukpt_aes/Format4PinBlock.cpp \
-L/opt/homebrew/opt/openssl/lib \
-lssl -lcrypto -Wno-deprecated-declarations \
-o DUKPT_Client

