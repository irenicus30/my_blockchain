#pragma once

#include "blockchain.h"
#include "block.h"


class connection_t;
typedef std::shared_ptr<connection_t> connection_ptr;

enum class message_type : uint32_t
{
    none,
    single_block = 1,
    history = 2,
    peers_info = 3,
};

struct input_message_t 
{
    enum { header_length = 8 };
    enum { max_body_length = 1024*1024 };

    input_message_t();
    input_message_t(input_message_t&);
    input_message_t(input_message_t&&);
    input_message_t& operator=(input_message_t&);
    input_message_t& operator=(input_message_t&&);

    bool parse_header();

    uint32_t body_length = 0;
    message_type type = message_type::none;

    byte_vector_t buffer;
    connection_ptr connection = nullptr;

    byte_t* body = nullptr;

    block_ptr get_single_block();
    int get_history(std::vector<block_ptr>& blocks);
};

struct output_message_t
{
    enum { header_length = sizeof(uint32_t) + sizeof(message_type) };
    enum { max_body_length = 1024*1024 };

    output_message_t(block_ptr block);
    output_message_t(int count, std::vector<block_ptr> blocks);

    uint32_t body_length = 0;
    message_type type = message_type::none;
    byte_t* body = nullptr;

    byte_vector_t buffer;

    connection_ptr connection = nullptr;
    bool broadcast = false;
};

typedef std::shared_ptr<input_message_t> input_message_ptr;
typedef std::shared_ptr<output_message_t> output_message_ptr;