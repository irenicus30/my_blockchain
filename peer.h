#pragma once

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>

#include <unordered_set>
#include <mutex>

#include "blockchain.h"
#include "peer_session.h"
#include "message.h"

using boost::asio::io_context;
using boost::asio::ip::tcp;

class peer_t {

    public:
        peer_t(boost::asio::io_context& io_context, const tcp::endpoint& endpoint);
        peer_t(peer_t&) = delete;
        peer_t& operator=(peer_t&) = delete;

        int broadcast_message(message_ptr message);
    
    private:

        void do_accept();

        tcp::acceptor acceptor_;
        std::unordered_set<peer_session_ptr> sessions;

        std::mutex sessions_mutex;
};