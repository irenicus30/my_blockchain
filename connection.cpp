#include "connection.h"

#include "server.h"


void connection_t::do_connect(const tcp::resolver::results_type endpoints)
{
    boost::asio::async_connect(
        socket,
        endpoints,
        [this](boost::system::error_code ec, tcp::endpoint endpoint)
        {
            BOOST_LOG_TRIVIAL(trace) <<  "connecting to " << endpoint.address().to_string() << " "
                << endpoint.port() << ((ec) ? " success" : " fail");
            if (!ec)
            {
                started = true;
                do_read_header();
            }
        });
}
void connection_t::do_read_header()
{
    input_message.buffer.resize(input_message_t::header_length);
    auto boost_buffer = boost::asio::buffer(input_message.buffer.data(), input_message_t::header_length);
    boost::asio::async_read(
        socket,
        boost_buffer,
        [this](boost::system::error_code ec, std::size_t /*length*/)
        {
            if (!ec)
            {
                bool parsing_result = input_message.parse_header();
                if(parsing_result == false) {
                    close();
                }
                else
                {
                    do_read_body(); 
                }               
            }
            else
            {
                close();
            }
        });
}

void connection_t::do_read_body()
{
    auto boost_buffer = boost::asio::buffer(input_message.buffer.data() + input_message_t::header_length, input_message.body_length);
    boost::asio::async_read(
        socket,
        boost_buffer,
        [this](boost::system::error_code ec, std::size_t /*length*/)
        {
            if (!ec)
            {
                input_message.connection = shared_from_this();
                message_queue_t& instance = message_queue_t::get_instance();
                instance.add_message(input_message);
                do_read_header();
            }
            else
            {
                close();
            }
        });
}

void connection_t::send(output_message_ptr message)
{
	std::lock_guard<std::recursive_mutex> lock(connection_lock);
    if(started == false || finished == true)
    {
        return;
    }

    bool no_pending_write = output_messages.empty();
    output_messages.push(message);
    if(no_pending_write)
    {
        internal_send(message);
    }
}

void connection_t::internal_send(output_message_ptr message)
{
    auto boost_buffer = boost::asio::buffer(message->buffer.data(), message->buffer.size());
    boost::asio::async_write(
        socket,
        boost_buffer,
        [message = std::move(message), this] (const boost::system::error_code& ec, size_t n) mutable
        {
            std::lock_guard<std::recursive_mutex> lock(connection_lock);
            boost::asio::ip::tcp::endpoint endpoint = socket.remote_endpoint();
            BOOST_LOG_TRIVIAL(trace) <<  "sending message to "
                << endpoint.address().to_string() << " "
                << endpoint.port() << ((ec) ? " success" : " fail");
            output_messages.pop();
            if(!output_messages.empty())
            {
                internal_send(output_messages.front());
            }                
        });
}

void connection_t::request_sync(int sync_root)
{
    
}

tcp::socket& connection_t::get_socket()
{
    return socket;
}

tcp::endpoint connection_t::get_remote_endpoint()
{
    return socket.remote_endpoint();
}


void connection_t::close()
{
    socket.close();
    finished = true;
}