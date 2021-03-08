#include <chain.h>

bool chain_t::add_block(block_t* block) {
    uint32_t number = block->this_block_number;

    if(number > blocks.size())
        return false;
    
    block_t* previous = nullptr;
    for(block_t* p : blocks[number-1]) {
        if(p->this_block_hash == block->previous_block_hash)
            previous = p;
    }
    if(previous == nullptr)
        return false;

    blocks[number].push_back(block);
    


}