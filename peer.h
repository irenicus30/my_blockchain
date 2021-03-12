#pragma once

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>

#include <unordered_set>
#include <mutex>
#include <memory>
#include <unordered_map>
#include <deque>
#include <algorithm>

#include "blockchain.h"
#include "message.h"
#include "loader.h"

using boost::asio::io_context;
using boost::asio::ip::tcp;

class peer_session_t;
typedef std::shared_ptr<peer_session_t> peer_session_ptr;

class peer_t : public std::enable_shared_from_this<peer_t> {

    public:
        peer_t(boost::asio::io_context& io_context, const tcp::endpoint& endpoint, loader_t& loader);
        peer_t(peer_t&) = delete;
        peer_t& operator=(peer_t&) = delete;

        int broadcast_message(message_ptr message);

        bool remove_message_from_map(message_ptr message);
        
        std::unordered_map<message_ptr, int> output_messages;
        std::mutex sessions_mutex;
        
        std::deque<message_ptr> input_messages;
        std::mutex input_message_mutex;

        loader_t& loader;
        std::unordered_set<peer_session_ptr> sessions;
    
    private:

        void connect();
        void do_accept();

        tcp::acceptor acceptor_;
        boost::asio::io_context& io_context;
};