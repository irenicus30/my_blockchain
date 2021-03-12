#include "loader.h"

#include <sstream>
#include <fstream>


loader_t::loader_t(std::string config_file_name/* = "" */) {
    BOOST_LOG_TRIVIAL(error) <<  "loading config from file " << config_file_name;
    if(!config_file_name.empty()) {
        std::ifstream is_file(config_file_name, std::ifstream::in);


        std::string line;
        while( std::getline(is_file, line) )
        {
            std::istringstream is_line(line);
            std::string key;
            if( std::getline(is_line, key, '=') )
            {
                std::string value;
                if( std::getline(is_line, value) ) 
                    store_line(key, value);
            }
        }
    }
    BOOST_LOG_TRIVIAL(error) <<  *this;
}


bool loader_t::store_line(std::string key, std::string value) {
    config_entry entry = resolve_options(key);
    switch(entry) {
        case config_entry::p2p_address:
            p2p_address = value;
            break;
        case config_entry::p2p_port:
            p2p_port = value;
            break;
        case config_entry::seed_address:
            seed_address.push_back(value);
            break;
        case config_entry::seed_port:
            seed_port.push_back(value);
            break;
        case config_entry::api_address:
            api_address = value;
            break;
        case config_entry::api_port:
            api_port = value;
            break;
        default:
            break;
    }

    return true;
}

config_entry loader_t::resolve_options(std::string s) {
    if(s == "p2p_address")
        return config_entry::p2p_address;
    if(s == "p2p_port")
        return config_entry::p2p_port;
    if(s == "seed_address")
        return config_entry::seed_address;
    if(s == "seed_port")
        return config_entry::seed_port;
    if(s == "api_address")
        return config_entry::api_address;
    if(s == "api_port")
        return config_entry::api_port;
    return config_entry::not_an_option;
}


std::ostream& operator<<(std::ostream& out, loader_t& loader) {
    out << "CONFIG\n";
    out << "\tp2p_address " << loader.p2p_address << "\n";
    out << "\tp2p_port " << loader.p2p_port << "\n";

    int seed_count = std::min(loader.seed_address.size(), loader.seed_port.size());
    for(int i = 0; i < seed_count; i++)
    {
        out << "\tseed_address " << loader.seed_address[i] << "\n";
        out << "\tseed_port " << loader.seed_port[i] << "\n";
    }

    out << "\tapi_address " << loader.api_address << "\n";
    out << "\tapi_port " << loader.api_port << "\n";

    return out;
}