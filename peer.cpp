#include "peer.h"

#include <memory>

#include "peer_session.h"

peer_t::peer_t(boost::asio::io_context& io_context,
        const tcp::endpoint& endpoint,
        loader_t& loader)
        : io_context(io_context), acceptor_(io_context, endpoint), loader(loader) {
    connect();
    do_accept();
}

void peer_t::connect() {
    int peers_number = std::min(loader.seed_address.size(), loader.seed_port.size());
    for(int i = 0; i < peers_number; i++) {
        tcp::resolver resolver(io_context);
        auto endpoints = resolver.resolve(loader.seed_address[i], loader.seed_port[i]);
        peer_session_ptr peer_session = std::make_shared<peer_session_t>(io_context, shared_from_this());
        peer_session->do_connect(endpoints);
    }
}

void peer_t::do_accept() {
    acceptor_.async_accept(
        [this](boost::system::error_code ec, tcp::socket socket)
        {
            if (!ec)
            {
                peer_session_ptr peer_session = std::make_shared<peer_session_t>(std::move(socket), shared_from_this());
                peer_session->start();
                std::lock_guard<std::mutex> lock(sessions_mutex);
                sessions.insert(peer_session);
            }
            do_accept();
        });
}

int peer_t::broadcast_message(message_ptr message) {
    int count = 0;
    std::lock_guard<std::mutex> lock(sessions_mutex);
    for(auto& session : sessions) {
        session->send(message);
    }
    if(sessions.size()>0)
        output_messages[message] = sessions.size();
    return sessions.size();
}


bool peer_t::remove_message_from_map(message_ptr message) {
    std::lock_guard<std::mutex> lock(sessions_mutex);
    output_messages[message]--;
    if(output_messages[message]==0)
        output_messages.erase(message);
    return true;
}