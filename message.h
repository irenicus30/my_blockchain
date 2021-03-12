#pragma once

#include "blockchain.h"
#include "block.h"


class message_t {
    public:
        enum { header_length = 4 };
        enum { max_body_length = 1024*1024 };
    public:
        message_t() {}
        message_t(block_ptr block);
        uint32_t body_size;
        byte_vector_t buffer;
};

typedef std::shared_ptr<message_t> message_ptr;