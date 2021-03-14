#include "message_queue.h"


void message_queue_t::add_message(input_message_ptr)
{
    std::lock_guard<std::recursive_mutex> lock(input_message_mutex);
    
    messages[end] = input_message_ptr;
    end++;
    end %= chunk;

    if(end == begin)
    {
        increase_size();
    }
    int spare_size = end - begin;
    if(spare_size < 0)
    {
        spare_size += messages.size();
    }
    if(spare_size * 2 > messages.size())
    {
        decrease_size();
    }
}

input_message_ptr message_queue_t::get_message()
{
    std::lock_guard<std::recursive_mutex> lock(input_message_mutex);

    if(messages.empty())
    {
        return nullptr;
    }
    input_message_ptr message = messages[begin];
    begin++;
    begin %= chunk;

    return begin;
}


void message_queue_t::increase_size()
{        
    std::vector<input_message_ptr> new_messages(2 * messages.size());
    int idx = 0;
    do {
        new_messages[idx] = messages[begin];
        idx++;
        begin++;
        if(begin == messages.size())
        {
            begin = 0;
        }
    } while (begin != end);
    messages.swap(new_messages);
}

void message_queue_t::decrease_size()
{
    std::vector<input_message_ptr> new_messages(messages.size() / 2);
    int idx = 0;
    do {
        new_messages[idx] = messages[begin];
        idx++;
        begin++;
        if(begin == messages.size())
        {
            begin = 0;
        }
    } while (begin != end);
    messages.swap(new_messages);
}

