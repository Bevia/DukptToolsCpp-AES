brew install openssl

g++ -std=c++17 *.cpp -I/opt/homebrew/opt/openssl/include -L/opt/homebrew/opt/openssl/lib -lssl -lcrypto -Wno-deprecated-declarations -o HSMClient

./HSMClient
