#include "server.h"

#include <memory>

#include "connection.h"

server_t::server_t(boost::asio::io_context& io_context,
        const tcp::endpoint& endpoint,
        loader_t& loader)
        : io_context(io_context), acceptor(io_context, endpoint), loader(loader) {
}

void server_t::setup()
{    
    BOOST_LOG_TRIVIAL(trace) <<  "creating server";
    connect_known_peers();
    BOOST_LOG_TRIVIAL(trace) <<  "connect done";
    do_accept();
    BOOST_LOG_TRIVIAL(trace) <<  "do_accept done";
}

void server_t::connect_known_peers() {
    int peers_number = std::min(loader.seed_address.size(), loader.seed_port.size());
    for(int i = 0; i < peers_number; i++) {
        BOOST_LOG_TRIVIAL(trace) <<  "trying to connect to known peer at " << loader.seed_address[i] << " " << loader.seed_port[i];
        tcp::resolver resolver(io_context);
        auto endpoints = resolver.resolve(loader.seed_address[i], loader.seed_port[i]);
        connection_ptr connection = create_connection();
        connection->do_connect(endpoints);
    }
}

void server_t::do_accept() {
    connection_ptr connection = create_connection();
    acceptor.async_accept(
        connection->get_socket(),
        [this, connection](boost::system::error_code ec)
        {
            tcp::endpoint endpoint = connection->get_remote_endpoint();
            BOOST_LOG_TRIVIAL(trace) <<  "accepting connection from " << endpoint.address().to_string() << " "
                << endpoint.port() << ((ec) ? " success" : " fail");
            if (!ec)
            {
                connection->started = true;
                connection->do_read_header();
            }
            do_accept();
        });
}

int server_t::broadcast_block(block_ptr block) {
    for(auto& connection : connections) {
        if(connection->finished)
        {
            connections.erase(connection);
        }
        else
        {
            output_message_ptr message = std::make_shared<output_message_t>(block);
            BOOST_LOG_TRIVIAL(trace) <<  "send block to peer";
            connection->send(std::move(message));
        }
    }
    return connections.size();
}


input_message_ptr server_t::receive_message()
{

    return nullptr;
}


connection_ptr create_connection()
{
    auto connection = std::make_shared<connection_t>(io_context);
    connections.insert(connection);
}