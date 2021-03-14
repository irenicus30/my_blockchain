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
#include "peer_session.h"

using boost::asio::ip::tcp;

class peer_t : public std::enable_shared_from_this<peer_t> {

    public:
        peer_t(boost::asio::io_context& io_context, const tcp::endpoint& endpoint, loader_t& loader);
        peer_t(peer_t&) = delete;
        peer_t& operator=(peer_t&) = delete;
        void setup();

        int broadcast_block(block_ptr block);
        input_message_ptr receive_message();

        loader_t& loader;
        std::unordered_set<peer_session_ptr> sessions;
    
    private:

        void connect();
        void do_accept();

        tcp::acceptor acceptor;
        boost::asio::io_context& io_context;
};