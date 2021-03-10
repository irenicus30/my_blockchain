#include <iostream>
#include <iomanip>
#include <exception>

#include <boost/program_options.hpp>

#include "blockchain.h"
#include "loader.h"

namespace po = boost::program_options;

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
    }
    catch(std::exception& e) {
        BOOST_LOG_TRIVIAL(error) <<  e.what();
    }

    return 0;
}