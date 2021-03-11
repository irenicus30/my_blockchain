#pragma once

#include <map>
#include <unordered_set>

#include "blockchain.h"
#include "block.h"

struct byte_vector_pointer_comparator {
    bool operator()(const byte_vector_t* a, const byte_vector_t* b) const {
        return *a < *b;
    }
};

struct chain_t {
    chain_t& get_instance() {
        static chain_t instance;
        return instance;
    }

    
    std::map<byte_vector_t*, block_t*, byte_vector_pointer_comparator> blocks;

    std::unordered_set<block_t*> heads;

    bool add_block(block_t* block);

    private:
        chain_t() {};
        chain_t(chain_t&) = delete;
        chain_t& operator=(chain_t&) = delete;

};
