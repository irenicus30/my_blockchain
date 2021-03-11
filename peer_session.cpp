#include "peer_session.h"

void peer_session_t::start() {
    do_read();
}

void peer_session_t::do_read() {

}

void peer_session_t::do_write() {
    
}

void peer_session_t::send(message_ptr message) {
    
    boost::asio::async_write(socket_,
                                boost::asio::buffer(message->buffer.data(), message->size),
                                [message, this](const boost::system::error_code& erro, std::size_t bytes_transferred ) {
                                });
}