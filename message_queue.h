#pragma once 

#include "blockchain.h"

#include <mutex>
#include <queue>

#include "message.h"

class message_queue_t
{
    enum { chunk = 100 };
    public:
        static message_queue_t& get_instance()
        {
            static message_queue_t instance;
            return instance;
        }

        void add_message(input_message_t);
        input_message_ptr get_message();

    private:
        message_queue_t() {}
        message_queue_t(message_queue_t&) = delete;
        message_queue_t& operator=(message_queue_t&) = delete;

        std::recursive_mutex input_message_mutex;
        std::queue<input_message_t> messages;
};