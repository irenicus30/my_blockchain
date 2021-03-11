#pragma once

#include <map>
#include <unordered_set>
#include <deque>
#include <mutex>
#include <chrono>

#include "blockchain.h"
#include "block.h"
#include "loader.h"
#include "peer.h"
#include "message.h"


struct chain_t {
    static chain_t& get_instance() {
        static chain_t instance;
        return instance;
    }

    
    std::map<byte_vector_t, block_ptr> blocks;

    std::unordered_set<block_ptr> fork_heads;
    block_ptr head = nullptr;
    peer_ptr peer;

    void run(loader_t& loader, peer_ptr ptr);

    private:
        chain_t() {};
        chain_t(chain_t&) = delete;
        chain_t& operator=(chain_t&) = delete;
        

        bool add_block(block_ptr block);

        bool cleanup_forks(block_ptr block);

        int broadcast_block(block_ptr block);

        bool mine(std::chrono::milliseconds millis);
        
        std::deque<message_ptr> input_messages;
        std::mutex deque_mutex;

};
