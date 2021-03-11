#include "message.h"

message_t::message_t(block_ptr block) {
    buffer = block->serialize();
    size = buffer.size();
}