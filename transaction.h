#include <blockchain.h>

struct transaction_t {
    uint16_t size;
    std::string data;

    std::string signature;
};