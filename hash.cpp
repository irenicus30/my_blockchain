#include "hash.h"


hash_t get_hash(byte_ptr v, int len)
{
    bool status = true;

    SHA256_CTX context;
    if(!SHA256_Init(&context))
        status = false;

    if(!SHA256_Update(&context, v, len))
        status = false;

    hash_t md;    
    if(!SHA256_Final(md.data(), &context))
        status = false;

    if(status == true)
    {
        return md;
    }
    else
    {
        return {0};
    }
}


bool verify_hash(byte_ptr v, int len, hash_t& original)
{
    hash_t computed = get_hash(v, len);

    return (original == computed);
}