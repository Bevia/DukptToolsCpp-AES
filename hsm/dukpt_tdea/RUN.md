g++ -std=c++17 \
./hsm/dukpt_tdea/DUKPT_Client.cpp \
./hsm/dukpt_tdea/DUKPTKeyDerivation.cpp \
./hsm/dukpt_tdea/Format0PinBlock.cpp \
./hsm/dukpt_tdea/DukptHsm.cpp \
-I. -I/opt/homebrew/opt/openssl/include \
-L/opt/homebrew/opt/openssl/lib \
-lssl -lcrypto -Wno-deprecated-declarations \
-o DUKPT_Client

./DUKPT_Client

