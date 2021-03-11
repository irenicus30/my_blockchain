#include "peer.h"

#include <memory>

peer_t::peer_t(boost::asio::io_context& io_context,
        const tcp::endpoint& endpoint)
        : acceptor_(io_context, endpoint) {
    do_accept();
}

void peer_t::do_accept() {
    acceptor_.async_accept(
        [this](boost::system::error_code ec, tcp::socket socket)
        {
            if (!ec)
            {
                auto ptr = std::make_shared<peer_session_t>(std::move(socket));
                ptr->start();
                std::lock_guard<std::mutex> lock(sessions_mutex);
                sessions.insert(ptr);
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
}