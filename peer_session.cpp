#include "peer_session.h"

#include "peer.h"

void peer_session_t::start() {
    do_read_header();
}


void peer_session_t::do_connect(const tcp::resolver::results_type& endpoints) {
    auto self(shared_from_this());
    boost::asio::async_connect(socket_, endpoints,
        [this, self](boost::system::error_code ec, tcp::endpoint)
        {
            if (!ec)
            {
                std::lock_guard<std::mutex> lock(peer->sessions_mutex);
                peer->sessions.insert(self);
                do_read_header();
            }
        });
}

void peer_session_t::do_read_header() {
    message_ptr message = std::make_shared<message_t>();
    message->buffer.resize(message_t::max_body_length);
    boost::asio::async_read(socket_,
        boost::asio::buffer(message->buffer.data(), message_t::header_length),
        [&message, this](boost::system::error_code ec, std::size_t /*length*/)
        {
            if (!ec)
            {
                uint32_t* ptr = reinterpret_cast<uint32_t*>(message->buffer.data());
                message->body_size = *ptr;
                if(message->body_size > message_t::max_body_length) {
                    message->body_size = 0;
                    socket_.close();
                }
                else
                {
                    message->buffer.resize(message->body_size);
                    do_read_body(message); 
                }               
            }
            else
            {
                socket_.close();
            }
        });
}

void peer_session_t::do_read_body(message_ptr message) {
    boost::asio::async_read(socket_,
        boost::asio::buffer(message->buffer.data() + message_t::header_length, message->body_size),
        [&message, this](boost::system::error_code ec, std::size_t /*length*/)
        {
            if (!ec)
            {
                std::lock_guard<std::mutex>(peer->input_message_mutex);
                peer->input_messages.push_back(message);
                do_read_header();
            }
            else
            {
                socket_.close();
            }
        });
}


void peer_session_t::send(message_ptr message) {    
    boost::asio::async_write(socket_,
                             boost::asio::buffer(message->buffer.data(), message->buffer.size()),
                             [&message, this] (const boost::system::error_code& e, size_t n) {
                                 peer->remove_message_from_map(message);
                             });
}
