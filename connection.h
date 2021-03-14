#pragma once

#include <boost/asio.hpp>

#include "blockchain.h"
#include "message.h"
#include "message_queue.h"
#include "server.h"

#include <queue>
#include <mutex>

using boost::asio::ip::tcp;

class connection_t : public std::enable_shared_from_this<connection_t>
{
    public:
        connection_t(boost::asio::io_context& io_context)
            : socket(io_context) {}

        void send(output_message_ptr message);
        void request_sync(int sync_root);
        void close();
    
    private:    
        tcp::socket socket;
        tcp::socket& get_socket();
        tcp::endpoint get_remote_endpoint();
        void internal_send(output_message_ptr message);
        void do_connect(const tcp::resolver::results_type endpoints);
        void do_read_header();
        void do_read_body();

        input_message_t input_message;
        std::queue<output_message_ptr> output_messages;

        std::recursive_mutex connection_lock;

        bool started = false;
        bool finished = false;

    friend class server_t;
};

typedef std::shared_ptr<connection_t> connection_ptr;