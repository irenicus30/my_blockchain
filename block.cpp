#include "block.h"


bool block_t::add_hash()
{
    byte_vector_t v = serialize();
    this_block_hash = get_hash(v.data(), v.size() - sizeof(hash_t));

    return true;
}

bool block_t::verify() const
{
    byte_vector_t v = serialize();
    bool result = verify_hash(v.data(), v.size() - sizeof(hash_t), this_block_hash);

    return result;
}

byte_vector_t block_t::serialize() const
{
    byte_vector_t v;
    v.resize(get_size());
    byte_ptr ptr = v.data();

    uint64_t* nounce_ptr = reinterpret_cast<uint64_t*>(ptr);
    *ptr64 = nounce;
    ptr += sizeof(uint64_t);

    uint32_t* this_block_number_ptr = reinterpret_cast<uint32_t*>(ptr);
    *this_block_number_ptr = this_block_number;
    ptr += sizeof(uint32_t);
    
    uint32_t* transactions_number_ptr = reinterpret_cast<uint32_t*>(ptr);
    *transactions_number_ptr = transactions_number;
    ptr += sizeof(uint32_t);

    for(const transaction_t& t : transactions) {
        byte_vector_t t_v = t.serialize();
        std::memcpy(ptr, t_v.data(), t_v.size());
        ptr += t_v.size();
    }

    std::memcpy(ptr, previous_block_hash.data(), sizeof(hash_t));
    ptr += sizeof(hash_t);

    std::memcpy(ptr, this_block_hash.data(), sizeof(hash_t));
    ptr += sizeof(hash_t);

    return v;
}

bool block_t::deserialize(byte_ptr ptr)
{
    uint64_t* nounce_ptr = reinterpret_cast<uint64_t*>(ptr);
    nounce = *ptr64;
    ptr += sizeof(uint64_t);

    uint32_t* this_block_number_ptr = reinterpret_cast<uint32_t*>(ptr);
    this_block_number = *this_block_number_ptr;
    ptr += sizeof(uint32_t);

    uint32_t* transactions_size_ptr = reinterpret_cast<uint32_t*>(ptr);
    transactions_size = *transactions_size_ptr;
    ptr += sizeof(uint32_t);

    for(int i = 0; i < transactions_size; i++)
    {
        transaction_t t;
        t.deserialize(ptr);
        ptr += t.get_size();
        transactions.push_back(t);
    }

    std::memcpy(previous_block_hash.data(), ptr, sizeof(hash_t));
    ptr += sizeof(hash_t);

    std::memcpy(this_block_hash.data(), ptr, sizeof(hash_t));
    ptr += sizeof(hash_t);

    return true;
}

uint32_t block_t::get_size() const
{
    return sizeof(uint64_t) + sizeof(uint32_t) + sizeof(uint32_t) + transactions_size + sizeof(hash_t) + sizeof(hash_t);
}

bool block_t::is_enough_zeros_in_hash(hash_t& difficulty)
{
    return (this_block_hash < difficulty);
}