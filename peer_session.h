#pragma once

#include <boost/asio.hpp>

#include "blockchain.h"
#include "message.h"
#include "message_queue.h"
#include "peer.h"

using boost::asio::ip::tcp;

class message_t;
typedef std::shared_ptr<message_t> message_ptr;

class peer_t;
typedef std::shared_ptr<peer_t> peer_ptr;

class peer_session_t : public std::enable_shared_from_this<peer_session_t> {
    public:
        peer_session_t(tcp::socket socket)
            : socket(std::move(socket)) {}
        peer_session_t(boost::asio::io_context& io_context)
            : socket(io_context) {}

        void start();
        void do_connect(const tcp::resolver::results_type& endpoints);
        void send(message_ptr message);
        void request_sync(int sync_root);
    
    private:    
        tcp::socket socket;
        void do_read_header();
        void do_read_body(message_ptr message);

        bool finished = false;

    friend class peer_t;
};

typedef std::shared_ptr<peer_session_t> peer_session_ptr;