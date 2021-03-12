#pragma once

#include <boost/asio.hpp>

#include "blockchain.h"
#include "message.h"

using boost::asio::ip::tcp;

class peer_t;
typedef std::shared_ptr<peer_t> peer_ptr;

class peer_session_t : public std::enable_shared_from_this<peer_session_t> {
    public:
        peer_session_t(tcp::socket socket, peer_ptr ptr)
            : socket_(std::move(socket)), peer(ptr) {}

        void start();
        void send(message_ptr message);
    
    private:    
        tcp::socket socket_;
        void do_read();
        void do_write();

        peer_ptr peer;
};

typedef std::shared_ptr<peer_session_t> peer_session_ptr;