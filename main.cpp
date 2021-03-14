#include <signal.h>
#include <boost/program_options.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/signal_set.hpp>
#define BOOST_STACKTRACE_USE_ADDR2LINE
#include <boost/stacktrace.hpp>
#include <boost/filesystem.hpp>

#include <iostream>
#include <iomanip>
#include <exception>
#include <thread>
#include <functional>
#include <fstream>

#include "blockchain.h"
#include "loader.h"
#include "chain.h"


namespace po = boost::program_options;
using boost::asio::ip::tcp;

void my_signal_handler(int signum)
{
    ::signal(signum, SIG_DFL);
    boost::stacktrace::safe_dump_to("./backtrace.dump");
    std::cout << boost::stacktrace::stacktrace() << std::endl;
    ::raise(SIGABRT);
}

int main(int argc, char* argv[])
{
    if (boost::filesystem::exists("./backtrace.dump"))
    {
        // there is a backtrace
        std::ifstream ifs("./backtrace.dump");

        boost::stacktrace::stacktrace st = boost::stacktrace::stacktrace::from_dump(ifs);
        std::cout << "Previous run crashed:\n" << st << std::endl;

        // cleaning up
        ifs.close();
        boost::filesystem::remove("./backtrace.dump");
    }

    try
    {
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

        if (vm.count("help"))
        {  
            std::cout << desc << "\n";
            return 0;
        }

        if (vm.count("config"))
        {  
            config_file_name = vm["config"].as<std::string>();
        }

        loader_t loader(config_file_name);
        
        boost::asio::io_context io_context(1);
        //boost::asio::signal_set signals(io_context, SIGINT, SIGTERM);
        //signals.async_wait([&](auto, auto){ io_context.stop(); });
        ::signal(SIGSEGV, &my_signal_handler);
        ::signal(SIGABRT, &my_signal_handler);

        tcp::endpoint endpoint(tcp::v4(), std::atoi(loader.p2p_port.c_str()));
        server_ptr server = std::make_shared<server_t>(io_context, endpoint, loader);
        server->setup();
        
        std::thread t([&io_context](){ io_context.run(); });

        chain_t& instance = chain_t::get_instance();
        instance.run(loader, server);

        io_context.stop();
        t.join();
    }
    catch(std::exception& e)
    {
        BOOST_LOG_TRIVIAL(error) <<  e.what();
    }

    return 0;
}