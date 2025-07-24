#pragma once

#include <vector>

class DukptAES {
public:
    static std::vector<uint8_t> deriveIPEK(const std::vector<uint8_t>& bdk,
                                           const std::vector<uint8_t>& ksn);

private:
    static std::vector<uint8_t> aesCmac(const std::vector<uint8_t>& key,
                                        const std::vector<uint8_t>& data);
};
