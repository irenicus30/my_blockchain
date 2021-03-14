#include "hash.h"


hash_t get_hash(byte_ptr* v, int len)
{
    SHA256_CTX context;
    if(!SHA256_Init(&context))
        return false;

    if(!SHA256_Update(&context, v, len))
        return false;

    hash_t md;    
    if(!SHA256_Final(md.data(), &context))
        return false;

    return md;
}


bool verify_hash(byte_ptr* v, int len, hash_t& original)
{
    hash_t computed = get_hash(v, len);

    return (original == computed);
}