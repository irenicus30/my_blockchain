#include "message.h"

bool input_message_t::parse_header()
{    
    uint32_t* body_length_ptr = reinterpret_cast<uint32_t*>(buffer.data());
    body_length = *body_length_ptr;
    message_type* type_ptr = reinterpret_cast<message_type*>(buffer.data() + sizeof(message_type));
    type = *type_ptr;

    if(body_length > max_body_length)
    {
        buffer.resize(0);
        buffer.shrink_to_fit();
        return false;
    }

    buffer.resize(header_length + body_length);
    body = buffer.data() + header_length;
    return true;
}

block_ptr input_message_t::get_single_block()
{
    byte_t* ptr = buffer.data();

    uint32_t* body_length_ptr = reinterpret_cast<uint32_t*>(ptr);
    body_length = *body_length_ptr;
    ptr += sizeof(uint32_t);

    message_type* type_ptr = reinterpret_cast<message_type*>(ptr);
    type = *type_ptr;
    ptr += sizeof(message_type);

    if(type != message_type::single_block)
    {
        BOOST_LOG_TRIVIAL(error) <<  "message is not message_type::single_block";
        return nullptr;
    }

    block_ptr block = std::make_shared<block_t>();
    body = ptr;
    block->deserialize(body);
    return block;
}

output_message_t::output_message_t(block_ptr block)
{
    byte_vector_t serialized = block->serialize();
    body_length = serialized.size();
    type = message_type::single_block;

    buffer.resize(header_length + body_length);
    byte_t* ptr = buffer.data();

    uint32_t* body_length_ptr = reinterpret_cast<uint32_t*>(ptr);
    *body_length_ptr = body_length;
    ptr += sizeof(uint32_t);

    message_type* type_ptr = reinterpret_cast<message_type*>(ptr);
    *type_ptr = type;
    ptr += sizeof(message_type);

    std::memcpy(ptr, serialized.data(), body_length);
}


output_message_t::output_message_t(int count, std::vector<block_ptr> blocks)
{
    body_length = 0;
    type = message_type::history;
}