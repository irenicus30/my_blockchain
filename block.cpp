#include <block.h>

void block_t::add_hash() {
    SHA256 hash;

    std::vector<byte> v = serialize();
    //32 is hash length and is not hashed
    hash.Update(v.data(), size-32);

    std::string digest;
    digest.resize(hash.DigestSize());
    hash.Final((byte*)&digest[0]);

}

bool block_t verify() {


    return true;
}