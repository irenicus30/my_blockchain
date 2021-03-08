#pragma once

#include <openssl/ec.h>
#include <openssl/ecdsa.h>
#include <openssl/obj_mac.h>

#include <blockchain.h>

struct transaction_t {
    uint16_t size;
    uint16_t data_size;
    std::string data;
    //signed up to here

    uint16_t signature_size;
    std::string signature;

    uint16_t private_key_size;
    std::string private_key;


    bool verify_data();
    bool verify_signature();

    std::vector<byte> serialize() const;
    bool deserialize(std::vector<byte>);

    int16_t get_size() const;
};