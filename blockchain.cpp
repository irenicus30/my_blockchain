#include "blockchain.h"


std::string to_string(const byte_vector_t& bytes) {
    std::stringstream ss;
    for(auto byte : bytes)
        if(std::isprint(byte))
            ss << byte;
        else
            ss << ".";

    return ss.str();
}
