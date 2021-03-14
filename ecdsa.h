#include "blockchain"

#include <openssl/ec.h>

byte_vector_t get_signature(byte_ptr*, int len, byte_vector_t& private_key);

bool verify_signature(byte_ptr*, int len, byte_vector_t& public_key, byte_vector_t& signature);