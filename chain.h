#pragma once

#include <map>
#include <unordered_set>

#include "blockchain.h"
#include "block.h"
#include "loader.h"
#include "server.h"
#include "connection.h"
#include "message.h"
#include "message_queue.h"
#include "miner.h"

class chain_t
{
    enum { blocks_to_sync = 60 };

    public:
        static chain_t& get_instance()
        {
            static chain_t instance;
            return instance;
        }

        
        std::map<hash_t, block_ptr> blocks;

        std::unordered_set<block_ptr> fork_heads;
        block_ptr head = nullptr;
        server_ptr server;

        void run(loader_t& loader, server_ptr ptr);

    private:
        chain_t() {};
        chain_t(chain_t&) = delete;
        chain_t& operator=(chain_t&) = delete;
        

        bool add_block(block_ptr block);

        bool sync(connection_ptr connection);

        bool cleanup_forks(block_ptr block);

        int broadcast_block(block_ptr block);

        block_ptr mine();

        miner_t miner;
};
