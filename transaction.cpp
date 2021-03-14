#include "transaction.h"


bool transaction_t::add_signature(byte_vector_t& private_key, byte_vector_t& public_key)
{
    public_key_size = public_key.size();
    this->public_key = public_key;

    byte_vector_t buffer(sizeof(uint16_t) + data_size);
    byte_ptr ptr = buffer.data();
    uint16_t* data_size_ptr = reinterpret_cast<uint16_t*>(ptr);
    *data_size_ptr = data_size;
    ptr += sizeof(uint16_t);
    std::memcpy(ptr, data.data(), data_size);
    ptr += data_size;

    signature = get_signature(buffer.data(), buffer.size(), private_key);

    signature_size = signature.size();
    return (signature_size != 0);
}

bool transaction_t::verify()
{
    if(!verify_data())
    {
        return false;
    }

    byte_vector_t buffer(sizeof(uint16_t) + data_size);
    byte_ptr ptr = buffer.data();
    uint16_t* data_size_ptr = reinterpret_cast<uint16_t*>(ptr);
    *data_size_ptr = data_size;
    ptr += sizeof(uint16_t);
    std::memcpy(ptr, data.data(), data_size);
    ptr += data_size;

    bool is_valid = verify_signature(buffer.data(), buffer.size(), public_key, signature);

    return is_valid;
}

byte_vector_t transaction_t::serialize() const
{
    byte_vector_t v;
    v.resize(get_size());
    byte_ptr ptr = v.data();

    uint16_t* data_size_ptr = reinterpret_cast<uint16_t*>(ptr);
    *data_size_ptr = data_size;
    ptr += sizeof(uint16_t);
    std::memcpy(ptr, data.data(), data_size);
    ptr += data_size;

    uint16_t* public_key_size_ptr = reinterpret_cast<uint16_t*>(ptr);
    *public_key_size_ptr = public_key_size;
    ptr += sizeof(uint16_t);
    std::memcpy(ptr, public_key.data(), public_key_size);
    ptr += public_key_size;

    uint16_t* signature_size_ptr = reinterpret_cast<uint16_t*>(ptr);
    *signature_size_ptr = signature_size;
    ptr += sizeof(uint16_t);
    std::memcpy(ptr, signature.data(), signature_size);
    ptr += signature_size;

    return v;
}

bool transaction_t::deserialize(byte_ptr ptr)
{
    uint16_t* data_size_ptr = reinterpret_cast<uint16_t*>(ptr);
    data_size = *data_size_ptr;
    ptr += sizeof(uint16_t);    
    data.resize(data_size);
    std::memcpy(data.data(), ptr, data_size);
    ptr += data_size;

    uint16_t* public_key_size_ptr = reinterpret_cast<uint16_t*>(ptr);
    public_key_size = *public_key_size_ptr;
    ptr += sizeof(uint16_t);    
    data.resize(public_key_size);
    std::memcpy(public_key.data(), ptr, public_key_size);
    ptr += public_key_size;

    uint16_t* signature_size_ptr = reinterpret_cast<uint16_t*>(ptr);
    signature_size = *signature_size_ptr;
    ptr += sizeof(uint16_t);    
    data.resize(signature_size);
    std::memcpy(signature.data(), ptr, signature_size);
    ptr += signature_size;

    transaction_hash = get_hash(ptr, get_size());

    return true;
}

bool transaction_t::verify_data() const
{
    return true;
}

uint16_t transaction_t::get_size() const
{
    return sizeof(uint16_t) + data_size + sizeof(uint16_t) + public_key_size + sizeof(uint16_t) + signature_size;
}
