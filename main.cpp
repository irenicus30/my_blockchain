#include <iostream>
#include <iomanip>
#include <exception>

#include <boost/program_options.hpp>

namespace po = boost::program_options;

int main(int argc, char* argv[]) {
    try {
        std::string cfile;
        po::options_description desc("Allowed options");
        desc.add_options()
        // First parameter describes option name/short name
        // The second is parameter to option
        // The third is description
        ("help,h", "print usage message")
        ("config,c", po::value(&cfile), "file with configuration")
        ;
        
        po::variables_map vm;
        po::store(parse_command_line(argc, argv, desc), vm);

        if (vm.count("help")) {  
            std::cout << desc << "\n";
            return 0;
        }

        if (vm.count("config")) {  
            cfile = vm["config"].as<std::string>();
        }
        std::cout << cfile << "\n";

        //loader_t loader();
    }
    catch(std::exception& e) {
        std::cerr << e.what() << "\n";
    }

    return 0;
}