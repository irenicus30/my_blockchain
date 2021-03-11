

#include <boost/program_options.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/signal_set.hpp>

#include <iostream>
#include <iomanip>
#include <exception>
#include <thread>
#include <functional>

#include "blockchain.h"
#include "loader.h"
#include "chain.h"

namespace po = boost::program_options;
using boost::asio::ip::tcp;

int main(int argc, char* argv[]) {
    try {
        std::string config_file_name;
        po::options_description desc("Allowed options");
        desc.add_options()
        // First parameter describes option name/short name
        // The second is parameter to option
        // The third is description
        ("help,h", "print usage message")
        ("config,c", po::value(&config_file_name), "file with configuration")
        ;
        
        po::variables_map vm;
        po::store(parse_command_line(argc, argv, desc), vm);

        if (vm.count("help")) {  
            std::cout << desc << "\n";
            return 0;
        }

        if (vm.count("config")) {  
            config_file_name = vm["config"].as<std::string>();
        }

        BOOST_LOG_TRIVIAL(info) << config_file_name;

        loader_t loader(config_file_name);
        
        boost::asio::io_context io_context(1);
        tcp::endpoint endpoint(tcp::v4(), std::atoi(loader.p2p_port.c_str()));
        peer_ptr ptr = std::make_shared<peer_t>(io_context, endpoint);
        boost::asio::signal_set signals(io_context, SIGINT, SIGTERM);
        signals.async_wait([&](auto, auto){ io_context.stop(); });

        chain_t& instance = chain_t::get_instance();
        auto fun = std::bind(&chain_t::run, &instance, loader, ptr);
        std::thread t(fun);
        t.detach();

        io_context.run();
    }
    catch(std::exception& e) {
        BOOST_LOG_TRIVIAL(error) <<  e.what();
    }

    return 0;
}