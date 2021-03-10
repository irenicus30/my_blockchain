#pragma once

#include <openssl/sha.h>

#include <blockchain.h>
#include <transaction.h>

struct block_t {
    uint32_t size;
    uint32_t nounce;
    uint32_t this_block_number;
    byte_array_t previous_block_hash;
    uint32_t transactions_size;
    std::vector<transaction_t> transactions;
    //hashed up to here

    byte_array_t this_block_hash;


    bool add_hash();
    bool verify();

    byte_array_t serialize() const;
    bool deserialize(byte_array_t);

    uint32_t get_size() const;

    block_t *prev = nullptr;
};