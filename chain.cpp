#include "chain.h"

#include "miner.h"

bool chain_t::add_block(block_ptr block)
{    
    auto it_this = blocks.find(block->this_block_hash);
    if(it_this != blocks.end())
    {
        BOOST_LOG_TRIVIAL(trace) << "this block with hash " << to_string(block->this_block_hash) << " already exists in blockchain";
        return false;
    }

    auto it_prev = blocks.find(block->previous_block_hash);
    if(block->this_block_number > 1 && it_prev == blocks.end())
    {
        BOOST_LOG_TRIVIAL(trace) << "previous block with hash " << to_string(block->previous_block_hash) << " does not exist in blockchain";
        return false;
    }

    if(block->this_block_number != 1 && it_prev->second->this_block_number + 1 != block->this_block_number)
    {
        BOOST_LOG_TRIVIAL(trace) << "number of block with hash " << to_string(block->previous_block_hash) << " does not match with previous block number";
        return false;
    }

    auto result = blocks.insert(std::pair<hash_t, block_ptr>(block->this_block_hash, block));
    if(!result.second)
    {
        return false;
    }
    BOOST_LOG_TRIVIAL(trace) << "inserting block";
    if(it_prev != blocks.end())
    {
        block->prev = it_prev->second;
    }
    else
    {
        block->prev = nullptr;
    }
    fork_heads.erase(block->prev);
    fork_heads.insert(block);

    if(head == nullptr || head->this_block_number < block->this_block_number)
    {
        head = block;        
    }

    BOOST_LOG_TRIVIAL(trace) << "broadcasting block";
    int count = server->broadcast_block(block);
    BOOST_LOG_TRIVIAL(trace) << "block broadcasted to " << count << " peers";

    return true;
}

bool chain_t::cleanup_forks(block_ptr block)
{
    return false;
}


void chain_t::run(loader_t& loader, server_ptr ser)
{
    server = ptr;

    // main loop
    while(true) {

        while(true) {
            message_queue_t& instance = message_queue_t::get_instance();
            input_message_ptr message = instance.get_message();
            if(message == nullptr)
            {
                break;
            }
            block_ptr block = message->get_single_block();
            if(block == nullptr)
            {
                continue;
            }
            BOOST_LOG_TRIVIAL(trace) <<  "received block";
            int result = add_block(block);
            if(result == false)
            {
                if(head==nullptr || block->this_block_number > head->this_block_number)
                {
                    sync(message->connection);
                }
            }
        }
        
        BOOST_LOG_TRIVIAL(trace) <<  "mining";
        using namespace std::literals::chrono_literals;
        block_ptr block = mine();
        if(block)
        {
            add_block(block);
        }
    }

}


bool chain_t::sync(connection_ptr connection)
{
    int sync_root = 0;
    if(head != nullptr)
        sync_root = head->this_block_number - blocks_to_sync;
    connection->request_sync(sync_root);
    return true;
}

block_ptr chain_t::mine()
{
    block_ptr block = miner.mine();
    return block;
}