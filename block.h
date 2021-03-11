#pragma once

#include "blockchain.h"
#include "transaction.h"

class block_t;
typedef std::shared_ptr<block_t> block_ptr;

struct block_t {
    uint32_t size;
    uint32_t nounce;
    uint32_t this_block_number;
    byte_vector_t previous_block_hash;
    uint32_t transactions_size;
    std::vector<transaction_t> transactions;
    //hashed up to here

    byte_vector_t this_block_hash;

    block_ptr prev = nullptr;

    bool add_hash();

    bool verify() const;

    byte_vector_t serialize() const;
    bool deserialize(byte_vector_t);

    uint32_t get_size() const;
};