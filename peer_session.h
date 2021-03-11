#pragma once

#include <boost/asio.hpp>

#include <unordered_set>
#include <functional>

#include "blockchain.h"
#include "message.h"

using boost::asio::ip::tcp;

class peer_session_t : public std::enable_shared_from_this<peer_session_t> {
    public:
        peer_session_t(tcp::socket socket)
            : socket_(std::move(socket)) {}

        void start();
        void send(message_ptr message);
    
    private:    
        tcp::socket socket_;
        void do_read();
        void do_write();
};

typedef std::shared_ptr<peer_session_t> peer_session_ptr;