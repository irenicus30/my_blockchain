#include "chain.h"

bool chain_t::add_block(block_t* block) {
    
    auto it_this = blocks.find(&block->this_block_hash);
    if(it_this != blocks.end()) {
        BOOST_LOG_TRIVIAL(info) << "this block with hash " << to_string(block->this_block_hash) << " already exists in blockchain";
        return false;
    }

    auto it_prev = blocks.find(&block->previous_block_hash);
    if(it_prev == blocks.end()) {
        BOOST_LOG_TRIVIAL(info) << "previous block with hash " << to_string(block->previous_block_hash) << " does not exist in blockchain";
        return false;
    }

    auto result = blocks.insert(std::pair<byte_vector_t*, block_t*>(&block->this_block_hash, block));


    return result.second;
}