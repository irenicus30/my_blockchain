#include "blockchain.h"

std::string print_binary(byte_t c) {
    std::stringstream ss;
    for (int i = 7; i >= 0; --i) {
        ss << ((c & (1 << i))? '1' : '0');
    }
    return ss.str();
}

std::string to_string(const byte_vector_t& bytes) {
    std::stringstream ss;
    for(auto byte : bytes)
        ss << print_binary(byte);

    return ss.str();
}
