#pragma once

#include "blockchain.h"
#include "block.h"


class message_t {
    public:
        message_t(block_ptr block);
        uint32_t size;
        byte_vector_t buffer;
};

typedef std::shared_ptr<message_t> message_ptr;