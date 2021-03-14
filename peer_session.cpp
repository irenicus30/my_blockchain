#include "peer_session.h"

#include "peer.h"

void peer_session_t::start()
{
    do_read_header();
}

void peer_session_t::do_connect(const tcp::resolver::results_type& endpoints)
{
    boost::asio::async_connect(socket, endpoints,
        [this](boost::system::error_code ec, tcp::endpoint endpoint)
        {
            BOOST_LOG_TRIVIAL(trace) <<  "connecting to " << endpoint.address.to_string() << " "
                << endpoint.port << (ec) ? " success" : " fail";
            if (!ec)
            {
                do_read_header();
            }
        });
}

void peer_session_t::do_read_header()
{
    input_message_ptr message = std::unique_ptr<input_message_t>();
    message->buffer.resize(input_message_t::header_length + input_message_t::max_body_length);
    boost::asio::async_read(socket,
        boost::asio::buffer(message->buffer.data(), input_message_t::header_length),
        [message = std::move(message), this](boost::system::error_code ec, std::size_t /*length*/)
        {
            if (!ec)
            {
                bool parsing_result = message->parse_header();
                if(parsing_result == false) {
                    socket.close();
                    finished = true;
                }
                else
                {
                    do_read_body(message); 
                }               
            }
            else
            {
                socket.close();
                finished = true;
            }
        });
}

void peer_session_t::do_read_body(input_message_ptr message)
{
    boost::asio::async_read(socket,
        boost::asio::buffer(message->buffer.data() + input_message_t::header_length, message->body_length),
        [message = std::move(message), this](boost::system::error_code ec, std::size_t /*length*/)
        {
            if (!ec)
            {
                message->peer_session = shared_from_this();
                message_queue_t& instance = message_queue_t.get_instance();
                instance.add_message(message);
                do_read_header();
            }
            else
            {
                socket.close();
                finished = true;
            }
        });
}


void peer_session_t::send(output_message_ptr message)
{    
    boost::asio::async_write(socket,
                             boost::asio::buffer(message->buffer.data(), message->buffer.size()),
                             [message = std::move(unique_ptr), this] (const boost::system::error_code& ec, size_t n) {
                                BOOST_LOG_TRIVIAL(trace) <<  "sending message to "
                                    << socket.remote_endpoint.address.to_string() << " "
                                    << socket.remote_endpoint.port << (ec) ? " success" : " fail";
                             });
}



void peer_session_t::request_sync(int sync_root)
{
    
}