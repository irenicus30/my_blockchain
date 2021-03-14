#pragma once

#include "blockchain.h"

#include <chrono>
#include <random>
#include <limits>
#include <chrono>

#include "block.h"
#include "transaction.h"

class miner_t
{
    public:
        block_ptr mine();
        block_ptr mine(std::chrono::milliseconds millis);

        std::chrono::milliseconds mining_time {1000};
        hash_t difficulty = {0, 0, 0, 255};
};