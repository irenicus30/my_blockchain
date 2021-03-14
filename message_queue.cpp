#include "message_queue.h"


void message_queue_t::add_message(input_message_t message)
{
    std::lock_guard<std::recursive_mutex> lock(input_message_mutex);
    
    messages.push(message);
}

input_message_ptr message_queue_t::get_message()
{
    std::lock_guard<std::recursive_mutex> lock(input_message_mutex);

    if(messages.empty())
    {
        return nullptr;
    }
    input_message_t message = messages.front();
    messages.pop();

    return std::make_shared<input_message_t>(message);
}
