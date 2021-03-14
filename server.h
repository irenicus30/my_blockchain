#pragma once

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>

#include <unordered_set>
#include <memory>
#include <unordered_map>
#include <deque>
#include <algorithm>

#include "blockchain.h"
#include "message.h"
#include "loader.h"
#include "connection.h"

using boost::asio::ip::tcp;

class server_t : public std::enable_shared_from_this<server_t> {

    public:
        server_t(boost::asio::io_context& io_context, const tcp::endpoint& endpoint, loader_t& loader);
        server_t(server_t&) = delete;
        server_t& operator=(server_t&) = delete;
        void setup();

        int broadcast_block(block_ptr block);
        input_message_ptr receive_message();

        loader_t& loader;
        std::unordered_set<connection_ptr> connections;
    
    private:

        void connect_known_peers();
        void do_accept();

        connection_ptr create_connection();

        tcp::acceptor acceptor;
        boost::asio::io_context& io_context;
};