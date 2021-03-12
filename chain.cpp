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

    // main loop
    while(true) {

        while(true) {
            std::unique_lock<std::mutex> lock(peer->input_message_mutex);
            if(peer->input_messages.empty())
            {
                break;
            }
            message_ptr message = peer->input_messages.front();
            peer->input_messages.pop_front();
            lock.unlock();
            block_ptr block = std::make_shared<block_t>();
            block->deserialize(message->buffer);
            add_block(block);
        }
        using namespace std::literals::chrono_literals;
        block_ptr block = mine(100ms);
        if(block)
        {
            add_block(block);
        }
    }

}

block_ptr chain_t::mine(std::chrono::milliseconds millis) {    
    const std::chrono::time_point<std::chrono::system_clock> now =
        std::chrono::system_clock::now();
    block_ptr block = std::make_shared<block_t>();

    block->size = 4+8+4+SHA256_DIGEST_LENGTH+4+SHA256_DIGEST_LENGTH;
    
    if(head==nullptr)
    {
        block->this_block_number = 1;
        block->previous_block_hash = byte_vector_t(SHA256_DIGEST_LENGTH, 0);
    }
    else
    {
        block->this_block_number = head->this_block_number + 1;
        block->previous_block_hash = head->this_block_hash;
    }
    //TODO: add transactions here
    block->transactions_size = 0;

    block->prev = head;

    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<unsigned int> distrib(0, std::numeric_limits<unsigned int>::max());
    uint64_t i = distrib(gen);
    i <<= 32;
    i += distrib(gen);
    while(now + millis < std::chrono::system_clock::now())
    {
        block->nounce = i;
        block->add_hash();
        //difficulty one per milion
        if(block->is_enough_zeros_in_hash(6))
            return block;
        i++;
    }

    return nullptr;
}