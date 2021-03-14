#pragma once 

#include "blockchain.h"

#include <mutex>
#include <vector>

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

        void add_message(input_message_ptr&&);
        input_message_ptr get_message();

    private:
        message_queue_t() : messages(chunk) {}
        message_queue_t(message_queue_t&) = delete;
        message_queue_t& operator=(message_queue_t&) = delete;

        void increase_size();
        void decrease_size();

        std::recursive_mutex input_message_mutex;
        std::vector<input_message_ptr> messages;
        int begin = 0;
        int end = 0;
};