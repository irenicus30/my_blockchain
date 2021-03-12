#include "message.h"

message_t::message_t(block_ptr block) {
    byte_vector_t body = block->serialize();
    body_size = buffer.size();
    buffer.resize(header_length + body_size);
    uint32_t* ptr = reinterpret_cast<uint32_t*>(buffer.data());
    *ptr = body_size;
    std::memcpy(buffer.data()+header_length, body.data(), body_size);
}