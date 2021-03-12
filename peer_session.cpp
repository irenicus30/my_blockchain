#include "peer_session.h"

#include "peer.h"

void peer_session_t::start() {
    do_read();
}

void peer_session_t::do_read() {

}

void peer_session_t::do_write() {
    
}

void peer_session_t::send(message_ptr message) {    
    boost::asio::async_write(socket_,
                             boost::asio::buffer(message->buffer.data(), message->buffer.size()),
                             [&message, this] (const boost::system::error_code& e, size_t n) {
                                 peer->remove_message_from_map(message);
                             });
}
