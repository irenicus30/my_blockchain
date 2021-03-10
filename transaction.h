#pragma once

#include <openssl/ec.h>
#include <openssl/ecdsa.h>
#include <openssl/obj_mac.h>

#include <blockchain.h>

struct transaction_t {
    uint16_t size;
    uint16_t data_size;
    byte_vector_t data;
    //signed up to here

    uint16_t private_key_size;
    byte_vector_t private_key;

    uint16_t signature_size;
    byte_vector_t signature;

    bool add_signature(byte_vector_t key);

    bool verify_data() const;
    bool verify_signature() const;

    byte_vector_t serialize() const;
    bool deserialize(byte_vector_t);

    int16_t get_size() const;
};