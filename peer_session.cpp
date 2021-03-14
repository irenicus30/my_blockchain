#include "peer_session.h"

#include "peer.h"

void peer_session_t::start()
{
    do_read_header();
}

void peer_session_t::do_connect(const tcp::resolver::results_type& endpoints)
{
    auto self(shared_from_this());
    boost::asio::async_connect(socket, endpoints,
        [this, self](boost::system::error_code ec, tcp::endpoint endpoint)
        {
            BOOST_LOG_TRIVIAL(trace) <<  "connecting to " << endpoint.address().to_string() << " "
                << endpoint.port() << ((ec) ? " success" : " fail");
            if (!ec)
            {
                do_read_header();
            }
        });
}

void peer_session_t::do_read_header()
{
    input_message_ptr message = std::make_shared<input_message_t>();
    message->buffer.resize(input_message_t::header_length + input_message_t::max_body_length);
    auto self(shared_from_this());
    boost::asio::async_read(socket,
        boost::asio::buffer(message->buffer.data(), input_message_t::header_length),
        [message = std::move(message), this, self](boost::system::error_code ec, std::size_t /*length*/) mutable
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
                    do_read_body(std::move(message)); 
                }               
            }
            else
            {
                socket.close();
                finished = true;
            }
        });
}

void peer_session_t::do_read_body(input_message_ptr&& message)
{
    auto self(shared_from_this());
    boost::asio::async_read(socket,
        boost::asio::buffer(message->buffer.data() + input_message_t::header_length, message->body_length),
        [message = std::move(message), this, self](boost::system::error_code ec, std::size_t /*length*/) mutable
        {
            if (!ec)
            {
                message->peer_session = shared_from_this();
                message_queue_t& instance = message_queue_t::get_instance();
                instance.add_message(std::move(message));
                do_read_header();
            }
            else
            {
                socket.close();
                finished = true;
            }
        });
}


void peer_session_t::send(output_message_ptr&& message)
{
    auto self(shared_from_this());
    auto buffer = message->buffer.data();
    auto size = message->buffer.size();
    boost::asio::async_write(socket,
        boost::asio::buffer(buffer, size),
        [message = std::move(message), this, self] (const boost::system::error_code& ec, size_t n) mutable
        {
        boost::system::error_code ec2;
        boost::asio::ip::tcp::endpoint endpoint = socket.remote_endpoint(ec2);
        BOOST_LOG_TRIVIAL(trace) <<  "sending message to "
            << endpoint.address().to_string() << " "
            << endpoint.port() << ((ec) ? " success" : " fail");
        });
}



void peer_session_t::request_sync(int sync_root)
{
    
}