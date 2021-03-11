#include "chain.h"

bool chain_t::add_block(block_ptr block) {
    
    auto it_this = blocks.find(block->this_block_hash);
    if(it_this != blocks.end()) {
        BOOST_LOG_TRIVIAL(trace) << "this block with hash " << to_string(block->this_block_hash) << " already exists in blockchain";
        return false;
    }

    auto it_prev = blocks.find(block->previous_block_hash);
    if(it_prev == blocks.end()) {
        BOOST_LOG_TRIVIAL(trace) << "previous block with hash " << to_string(block->previous_block_hash) << " does not exist in blockchain";
        return false;
    }

    if((it_prev->second == nullptr && block->this_block_number==1) ||
        (it_prev->second->this_block_number + 1 != block->this_block_number)) {
        BOOST_LOG_TRIVIAL(trace) << "number of block with hash " << to_string(block->previous_block_hash) << " does not match with previous block number";
        return false;
    }

    auto result = blocks.insert(std::pair<byte_vector_t, block_ptr>(block->this_block_hash, block));
    if(!result.second) {
        return false;
    }
    block->prev = it_prev->second;
    fork_heads.erase(block->prev);
    fork_heads.insert(block);

    if(head == nullptr || head->this_block_number < block->this_block_number) {
        head = block;        
    }

    broadcast_block(block);

    return true;
}

bool chain_t::cleanup_forks(block_ptr block) {
    return false;
}

int chain_t::broadcast_block(block_ptr block) {
    message_ptr message = std::make_shared<message_t>(block);
    int count = peer->broadcast_message(message);
    return count;
}


void chain_t::run(loader_t& loader, peer_ptr ptr) {
    peer = ptr;

    while(true) {

        while(true) {
            std::unique_lock<std::mutex> lock(deque_mutex);
            if(input_deque.empty())
                break;
            message_ptr message = input_deque.front();
            input_deque.pop_front();
            lock.unlock();
            block_ptr block = std::make_shared<block_t>();
            block->deserialize(message->buffer);
            add_block(block);
        }
        using namespace std::literals::chrono_literals;
        mine(100ms);

    }

}

bool chain_t::mine(std::chrono::milliseconds millis) {
    return true;
}