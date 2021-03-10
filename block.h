#pragma once

#include <openssl/sha.h>

#include <blockchain.h>
#include <transaction.h>

struct block_t {
    uint32_t size;
    uint32_t nounce;
    uint32_t this_block_number;
    byte_vector_t previous_block_hash;
    uint32_t transactions_size;
    std::vector<transaction_t> transactions;
    //hashed up to here

    byte_vector_t this_block_hash;

    block_t *prev = nullptr;

    bool add_hash();

    bool verify() const;

    byte_vector_t serialize() const;
    bool deserialize(byte_vector_t);

    uint32_t get_size() const;
};