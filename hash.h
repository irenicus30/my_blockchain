#include "blockchain"

#include <openssl/sha.h>

typedef std::array<byte_t, SHA256_DIGEST_LENGTH> hash_t;

hash_t get_hash(byte_ptr* ptr, int len);

bool verify_hash(byte_ptr* ptr, int len, hash_t& md);