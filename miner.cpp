#include "miner.h"

#include "chain.h"


block_ptr miner_t::mine()
{
    return mine(mining_time);
}

block_ptr miner_t::mine(std::chrono::milliseconds millis)
{
    chain_t& chain = chain_t::get_instance();
    const std::chrono::time_point<std::chrono::system_clock> now =
        std::chrono::system_clock::now();
    block_ptr block = std::make_shared<block_t>();
    
    if(chain.head==nullptr)
    {
        block->this_block_number = 1;
    }
    else
    {
        block->this_block_number = chain.head->this_block_number + 1;
    }
    //TODO: add transactions here
    block->transactions_number = 0;
    if(chain.head==nullptr)
    {
        block->previous_block_hash = hash_t{0};
    }
    else
    {
        block->previous_block_hash = chain.head->this_block_hash;
    }

    block->prev = chain.head;

    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<unsigned int> distrib(0, std::numeric_limits<unsigned int>::max());
    uint64_t i = distrib(gen);
    i <<= sizeof(unsigned int);
    i += distrib(gen);
    while(now + millis > std::chrono::system_clock::now())
    {
        block->nounce = i;
        block->add_hash();
        //difficulty one per milion
        if(block->is_enough_zeros_in_hash(difficulty))
        {
            BOOST_LOG_TRIVIAL(trace) <<  "found hash\n" << to_string(block->this_block_hash);
            return block;
        }
        i++;
    }
    return nullptr;
}