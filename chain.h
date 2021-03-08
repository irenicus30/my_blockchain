#pragma once

#include <blockchain.h>
#include <block.h>


struct chain_t {
    chain_t& get_instance() {
        static chain_t instance;
        return instance;
    }

    
    std::vector<std::vector<block_t*>> blocks;

    bool  add_block(block_t* block);

    private:
        chain_t() {};
        chain_t(chain_t&) = delete;
        chain_t& operator=(chain_t&) = delete;

};
