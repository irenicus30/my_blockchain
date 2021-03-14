#include "blockchain.h"

#include <iomanip>

#include "hash.h"


std::string to_string(const hash_t& bytes) {
    std::stringstream ss;
    for(auto byte : bytes)
        ss << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(byte);

    return ss.str();
}
