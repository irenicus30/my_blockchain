#include <block.h>

bool block_t::add_hash() {
    byte_vector_t v = serialize();

    SHA256_CTX context;
    if(!SHA256_Init(&context))
        return false;

    if(!SHA256_Update(&context, v.data(), size - SHA256_DIGEST_LENGTH))
        return false;

    byte_t md[SHA256_DIGEST_LENGTH];    
    if(!SHA256_Final(md, &context))
        return false;

    this_block_hash.assign(md, md + SHA256_DIGEST_LENGTH);

    return true;
}

bool block_t::verify() {
    SHA256_CTX context;
    if(!SHA256_Init(&context))
        return false;

    byte_vector_t v = serialize();

    if(!SHA256_Update(&context, v.data(), size - SHA256_DIGEST_LENGTH))
        return false;

    byte_t md[SHA256_DIGEST_LENGTH];    
    if(!SHA256_Final(md, &context))
        return false;

    byte_vector_t hash;
    std::memcpy(hash.data(), md, SHA256_DIGEST_LENGTH);

    if(hash == this_block_hash)
        return true;
    else
        return false;
}

byte_vector_t block_t::serialize() const {
    byte_vector_t v;
    v.resize(size);

    uint32_t *ptr = reinterpret_cast<uint32_t*>(&v[0]);
    *ptr = size;
    ptr = reinterpret_cast<uint32_t*>(&v[4]);
    *ptr = nounce;
    ptr = reinterpret_cast<uint32_t*>(&v[4+4]);
    *ptr = this_block_number;

    byte_t *byte_ptr = &v[4+4+4];
    std::memcpy(byte_ptr, previous_block_hash.data(), previous_block_hash.size());
    
    ptr = reinterpret_cast<uint32_t*>(&v[4+4+4+SHA256_DIGEST_LENGTH]);
    *ptr = transactions_size;
    byte_ptr = &v[4+4+4+SHA256_DIGEST_LENGTH+4];
    for(const transaction_t& t : transactions) {
        byte_vector_t t_v = t.serialize();
        std::memcpy(byte_ptr, t_v.data(), t_v.size());
        byte_ptr += t_v.size();
    }

    byte_ptr = &v[4+4+4+SHA256_DIGEST_LENGTH+4+transactions_size];
    std::memcpy(byte_ptr, this_block_hash.data(), this_block_hash.size());

    return v;
}

bool block_t::deserialize(std::vector<byte> v) {
    uint32_t *ptr = reinterpret_cast<uint32_t*>(&v[0]);
    size = *ptr;

    ptr = reinterpret_cast<uint32_t*>(&v[4]);
    nounce = *ptr;

    ptr = reinterpret_cast<uint32_t*>(&v[4+4]);
    this_block_number = *ptr;

    byte_t *byte_ptr = (&v[4+4+4];
    previous_block_hash.resize(SHA256_DIGEST_LENGTH);
    std::memcpy(previous_block_hash.data(), byte_ptr, SHA256_DIGEST_LENGTH);

    ptr = reinterpret_cast<uint32_t*>(&v[4+4+4+SHA256_DIGEST_LENGTH]);
    transactions_size = *ptr;

    int offset = 4+4+4+SHA256_DIGEST_LENGTH+4;
    int cumulative_size = 0;
    while(cumulative_size < transactions_size) {
        transactions.resize(transactions.size()+1);
        uint16_t *transaction_size_ptr = reinterpret_cast<uint16_t*>(&v[4+4+4+SHA256_DIGEST_LENGTH+4+offset+cumulative_size]);
        byte_vector_t serialized_transaction(&v[offset+cumulative_size], &v[offset+cumulative_size+*transaction_size_ptr]);
        transactions[transactions.size()-1].deserialize(serialized_transaction);
        cumulative_size += transactions[transactions.size()-1].size();
    }
    
    byte_ptr = &v[4+4+4+SHA256_DIGEST_LENGTH+4+transactions_size];
    this_block_hash.resize(SHA256_DIGEST_LENGTH);
    std::memcpy(this_block_hash.data(), byte_ptr, SHA256_DIGEST_LENGTH);

    return true;
}