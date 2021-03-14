#pragma once

#include "blockchain.h"

#include <openssl/ec.h>
#include <openssl/obj_mac.h>

byte_vector_t get_signature(byte_ptr, int len, byte_vector_t& private_key);

bool verify_signature(byte_ptr, int len, byte_vector_t& public_key, byte_vector_t& signature);