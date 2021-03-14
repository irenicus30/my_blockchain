#include "peer.h"

#include <memory>

#include "peer_session.h"

peer_t::peer_t(boost::asio::io_context& io_context,
        const tcp::endpoint& endpoint,
        loader_t& loader)
        : io_context(io_context), acceptor(io_context, endpoint), loader(loader) {
}

void peer_t::setup()
{    
    BOOST_LOG_TRIVIAL(trace) <<  "creating peer";
    connect();
    BOOST_LOG_TRIVIAL(trace) <<  "connect done";
    do_accept();
    BOOST_LOG_TRIVIAL(trace) <<  "do_accept done";
}

void peer_t::connect() {
    int peers_number = std::min(loader.seed_address.size(), loader.seed_port.size());
    for(int i = 0; i < peers_number; i++) {
        BOOST_LOG_TRIVIAL(trace) <<  "trying to connect to " << loader.seed_address[i] << " " << loader.seed_port[i];
        tcp::resolver resolver(io_context);
        auto endpoints = resolver.resolve(loader.seed_address[i], loader.seed_port[i]);

        peer_session_ptr peer_session = std::make_shared<peer_session_t>(io_context);
        sessions.insert(peer_session);
        peer_session->do_connect(endpoints);
    }
}

void peer_t::do_accept() {
    auto self(shared_from_this());
    acceptor.async_accept(
        [this, self](boost::system::error_code ec, tcp::socket socket)
        {
            boost::system::error_code ec2;
            boost::asio::ip::tcp::endpoint endpoint = socket.remote_endpoint(ec2);
            BOOST_LOG_TRIVIAL(trace) <<  "accepting connection from " << endpoint.address().to_string() << " "
                << endpoint.port() << ((ec) ? " success" : " fail");
            if (!ec)
            {
                peer_session_ptr peer_session = std::make_shared<peer_session_t>(std::move(socket));
                sessions.insert(peer_session);
                peer_session->start();
            }
            do_accept();
        });
}

int peer_t::broadcast_block(block_ptr block) {
    for(auto& session : sessions) {
        if(session->finished)
        {
            sessions.erase(session);
        }
        else
        {
            output_message_ptr message = std::make_shared<output_message_t>(block);
            BOOST_LOG_TRIVIAL(trace) <<  "send to peer";
            session->send(std::move(message));
        }
    }
    return sessions.size();
}


input_message_ptr peer_t::receive_message()
{

    return nullptr;
}

