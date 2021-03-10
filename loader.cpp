#include "loader.h"

#include <sstream>
#include <fstream>


loader_t::loader_t(std::string config_file_name /* = "" */) {
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
}


bool loader_t::store_line(std::string key, std::string value) {

    return true;
}