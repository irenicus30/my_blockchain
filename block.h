#pragma once

#include <openssl/sha.h>

#include <blockchain.h>
#include <transaction.h>

struct block_t {
    uint32_t size;
    uint32_t nounce;
    uint32_t this_block_number;
    std::string previous_block_hash;
    uint32_t transactions_size;
    std::vector<transaction_t> transactions;
    //hashed up to here

    std::string this_block_hash;


    bool add_hash();
    bool verify();

    std::vector<byte> serialize() const;
    bool deserialize(std::vector<byte>);

    uint32_t get_size() const;

    block_t *prev = nullptr;
};