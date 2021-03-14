#pragma once

#include "blockchain.h"
#include "transaction.h"
#include "hash.h"

struct block_t;
typedef std::shared_ptr<block_t> block_ptr;

struct block_t
{
    uint64_t nounce;
    uint32_t this_block_number;
    uint32_t transactions_number;
    std::vector<transaction_t> transactions;
    hash_t previous_block_hash;
    //hashed up to here

    hash_t this_block_hash;
    //serialized up here with vectors as byte arrays

    block_ptr prev = nullptr;

    bool add_hash();
    bool verify() const;

    byte_vector_t serialize() const;
    bool deserialize(byte_ptr);

    uint32_t get_size() const;
    bool is_enough_zeros_in_hash(hash_t& difficulty);
};