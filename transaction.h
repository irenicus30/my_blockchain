#pragma once

#include "blockchain.h"
#include "ecdsa.h"
#include "hash.h"

struct transaction_t
{
    uint16_t data_size;
    byte_vector_t data;
    //signed up to here

    uint16_t public_key_size;
    byte_vector_t public_key;

    uint16_t signature_size;
    byte_vector_t signature;
    //serialized up here with vectors as byte arrays

    hash_t transaction_hash;

    // add only in wallet
    bool add_signature(byte_vector_t& key);
    bool verify() const;

    byte_vector_t serialize() const;
    bool deserialize(byte_ptr);

    bool verify_data() const;
    uint16_t get_size() const;
};